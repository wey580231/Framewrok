#include "multidataforwardclients.h"

#include <QTimer>
#include <QDebug>

#include <base/util/rutil.h>
#include <network/reactor/eventloop.h>
#include <network/reactor/rtcpclient.h>

namespace Related{

SingleForwardClient::SingleForwardClient(Network::EventLoop *eventLoop, Network::NetAddress remoteAddress):m_eventLoop(eventLoop),m_remoteAddress(remoteAddress),
    m_reconnectTimer(nullptr),m_tcpClient(nullptr),m_iTryConnectTimes(0),m_bConnectorRunningFlag(true),
    m_recvBuffer(M_BYTES(1))
{
    init();
}

SingleForwardClient::~SingleForwardClient()
{
    m_bConnectorRunningFlag = false;
    m_cv.notify_one();
}

void SingleForwardClient::init()
{
    m_reconnectTimer = new QTimer();
    m_reconnectTimer->setInterval(6000);

    m_tcpClient = new Network::RTcpClient(m_eventLoop,m_remoteAddress);
    m_tcpClient->setConnectionCallback(std::bind(&SingleForwardClient::connectedToRemoteServer,this,std::placeholders::_1));
    m_tcpClient->setCloseCallback(std::bind(&SingleForwardClient::disconnectedFromServer,this,std::placeholders::_1));
    m_tcpClient->setMessageCallback(std::bind(&SingleForwardClient::recvMessageFromServer,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3));

    connect(m_reconnectTimer,SIGNAL(timeout()),this,SLOT(reconnectTimeout()),Qt::DirectConnection);

    //启动重连定时器，避免在数据到来时多次调用startConnect()造成重复连接。
    m_reconnectTimer->start();

    connect(this,SIGNAL(startConnectTimer()),this,SLOT(startConnect()),Qt::QueuedConnection);
    connect(this,SIGNAL(netConnected(bool)),this,SLOT(respNetConnected(bool)),Qt::QueuedConnection);

    //创建主动连接线程
    m_connectThread = std::move(std::thread(&SingleForwardClient::reconnectServer,this));
}

/*!
* @brief 线程连接远程服务器
*/
void SingleForwardClient::reconnectServer()
{
    while(m_bConnectorRunningFlag){
        if(!m_tcpClient->connected())
            m_tcpClient->connect();

        std::unique_lock<std::mutex> ul(m_connectMutex);
        m_cv.wait(ul);
    }
}

void SingleForwardClient::connectedToRemoteServer(const Network::TcpConnectionPtr& connectedConn)
{
    qDebug()<<QString(QStringLiteral("网络已连接，共尝试%1次.")).arg(m_iTryConnectTimes);
    m_bIsConnected = true;
    {
        std::lock_guard<std::mutex> lg(m_connectionMutex);
        qDebug()<<"SingleForwardClient::connectedToRemoteServer:"<<m_remoteConnection.get()<<m_remoteConnection.use_count()<<connectedConn.use_count();
        m_remoteConnection = connectedConn;
    }
    m_iTryConnectTimes = 0;

    emit netConnected(true);
}

/*!
 * @brief 从远程服务器断开连接
 * @param[in] connectedConn 数据连接
 */
void SingleForwardClient::disconnectedFromServer(const Network::TcpConnectionPtr &connectedConn)
{
    m_bIsConnected = false;
    m_recvBuffer.reset();
    emit startConnectTimer();
}

/*!
 * @brief 接收服务器下发的指令，并做处理
 * @param[in] connectedConn 接收网络数据的连接
 * @param[in] data 数据起始地址
 * @param[in] dataLen 数据长度
 */
void SingleForwardClient::recvMessageFromServer(const Network::TcpConnectionPtr &connectedConn, const char *data, int dataLen)
{
    m_recvBuffer.append(data,dataLen);

    if(m_dataParse){
        m_dataParse(connectedConn,m_recvBuffer);
    }
}

/*!
* @brief  网络状态改变异步调用
* @param[in] flag true:状态连接成功，false：连接失败
*/
void SingleForwardClient::respNetConnected(bool flag)
{
    if(flag)
        m_reconnectTimer->stop();
    else
        m_reconnectTimer->start();
}

/*!
 * @brief 转发数据至远程服务器端
 * @param[in]  dataList 待转发数据集合
 */
void SingleForwardClient::sendData(std::list<Core::Datastruct::ProtocolArray> &dataList)
{
    std::lock_guard<std::mutex> lg(m_connectionMutex);
    std::for_each(dataList.begin(),dataList.end(),[&](Core::Datastruct::ProtocolArray & array){
        if(m_bIsConnected && m_remoteConnection.get()){
            m_remoteConnection->send(array.data->data(),array.data->length());
        }
    });
}

void SingleForwardClient::send(const char *data, size_t len)
{
    std::lock_guard<std::mutex> lg(m_connectionMutex);
    if(m_bIsConnected && m_remoteConnection.get())
    {
        m_remoteConnection->send(data,len);
    }
}

/*!
 * @brief 主动尝试连接至远程服务器
 * @details 若定时器已经启动，说明在此之前已经断开连接，在等待定时器超时重连，避免多次调用连接
 */
void SingleForwardClient::startConnect()
{
    if(!m_bIsConnected && !m_reconnectTimer->isActive()){
        reconnectTimeout();
        emit netConnected(false);
    }
}

/*!
 * @brief 重连接定时器超时，需再次连接
 * @details 为了降低每次数据连接带来的处理上的堆积，在断网状态下，每次间隔一定时间进行重新连接。
 */
void SingleForwardClient::reconnectTimeout()
{
    m_cv.notify_one();
    //qDebug()<<QString(QStringLiteral("正在尝试%1次连接[%2:%3]...")).arg(m_iTryConnectTimes++).arg(m_remoteAddress.ip).arg(m_remoteAddress.port);
}



MultiForwardClients::MultiForwardClients(Network::EventLoop *eventloop):m_eventLoop(eventloop)
{    

}

MultiForwardClients::~MultiForwardClients()
{

}

void MultiForwardClients::init()
{
    for(Network::NetAddress address:m_forwardServerList){
        ClinetPtr ptr(new ForwardClient);
        ptr->m_client = new SingleForwardClient(m_eventLoop,address);
        ptr->m_client->setDataParse(m_defaultParse);
        connect(ptr->m_client,SIGNAL(netConnected(bool)),ptr.data(),SLOT(respNetConnected(bool)));

        m_forwardClients.append(ptr);
    }
}

bool MultiForwardClients::anyConnected()
{
    return std::any_of(m_forwardClients.begin(),m_forwardClients.end(),[&](ClinetPtr ptr){
        return ptr->m_client->connected() == true;
    });
}

void MultiForwardClients::startConnect()
{
    std::for_each(m_forwardClients.begin(),m_forwardClients.end(),[&](ClinetPtr ptr){
        ptr->startConnect();
    });
}

/*!
 * @brief 绑定转发server的网络信息
 * @param[in]  serverAddressInfos 服务器网络信息
 */
void MultiForwardClients::setServerInfo(QList<Network::NetAddress> && serverAddressInfos)
{
    m_forwardServerList = std::move(serverAddressInfos);

    std::call_once(m_onceForward,&MultiForwardClients::init,this);
}

void MultiForwardClients::sendData(const char *data, int length)
{
    std::for_each(m_forwardClients.begin(),m_forwardClients.end(),[&](ClinetPtr ptr){
        ptr->m_client->send(data,length);
    });
}


} //namespace Related{
