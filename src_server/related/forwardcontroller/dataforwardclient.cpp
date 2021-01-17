#include "dataforwardclient.h"

#include <QTimer>
#include <QDebug>

#include <base/util/rutil.h>
#include <network/reactor/eventloop.h>
#include <network/reactor/rtcpclient.h>

namespace Related{

DataForwardClient::DataForwardClient(Network::NetAddress remoteAddress):m_eventLoop(nullptr),m_bIsConnected(false),m_reconnectTimer(nullptr),m_tcpClient(nullptr),
    m_iTryConnectTimes(0),m_bConnectorRunningFlag(true)
{    
    m_remoteAddress = remoteAddress;
    init();
}

DataForwardClient::~DataForwardClient()
{
    m_bConnectorRunningFlag = false;
    m_cv.notify_one();
}

/*!
 * @brief 主动尝试连接至远程服务器
 * @details 若定时器已经启动，说明在此之前已经断开连接，在等待定时器超时重连，避免多次调用连接
 */
void DataForwardClient::startConnect()
{
    if(!m_bIsConnected && !m_reconnectTimer->isActive()){
        reconnectTimeout();
        emit netConnected(false);
    }
}

/*!
* @brief  网络状态改变异步调用
* @param[in] flag true:状态连接成功，false：连接失败
*/
void DataForwardClient::respNetConnected(bool flag)
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
void DataForwardClient::sendData(std::list<Core::Datastruct::ProtocolArray> &dataList)
{
    std::for_each(dataList.begin(),dataList.end(),[&](Core::Datastruct::ProtocolArray & array){
        if(m_remoteConnection.get()){
            qDebug()<<"*********"<<array.data->length();
            m_remoteConnection->send(array.data->data(),array.data->length());
        }
    });
}

void DataForwardClient::send(char *data, size_t len)
{
    if(m_remoteConnection.get())
    {
        m_remoteConnection->send(data,len);
    }
}

void DataForwardClient::connectedToRemoteServer(const Network::TcpConnectionPtr& connectedConn)
{
    qDebug()<<QString(QStringLiteral("网络已连接，共尝试%1次.")).arg(m_iTryConnectTimes);

    m_bIsConnected = true;
    m_remoteConnection = connectedConn;
    m_iTryConnectTimes = 0;

    emit netConnected(true);
}

/*!
 * @brief 从远程服务器断开连接
 * @param[in] connectedConn 数据连接
 */
void DataForwardClient::disconnectedFromeServer(const Network::TcpConnectionPtr &connectedConn)
{
    m_bIsConnected = false;
    emit startConnectTimer();
}

/*!
 * @brief 重连接定时器超时，需再次连接
 * @details 为了降低每次数据连接带来的处理上的堆积，在断网状态下，每次间隔一定时间进行重新连接。
 */
void DataForwardClient::reconnectTimeout()
{
    m_cv.notify_one();
    qDebug()<<QString(QStringLiteral("正在尝试%1次连接[%2]...")).arg(m_iTryConnectTimes++).arg(m_remoteAddress.ip)<<m_remoteAddress.port;
}

void DataForwardClient::init()
{
    if(m_reconnectTimer == nullptr)
    {
        m_reconnectTimer = new QTimer();
        m_reconnectTimer->setInterval(6000);

        if(m_eventLoop == nullptr)
            m_eventLoop = new Network::EventLoop();

        if(m_tcpClient == nullptr){
            m_tcpClient = new Network::RTcpClient(m_eventLoop,m_remoteAddress);
            m_tcpClient->setConnectionCallback(std::bind(&DataForwardClient::connectedToRemoteServer,this,std::placeholders::_1));
            m_tcpClient->setCloseCallback(std::bind(&DataForwardClient::disconnectedFromeServer,this,std::placeholders::_1));
        }
        connect(m_reconnectTimer,SIGNAL(timeout()),this,SLOT(reconnectTimeout()),Qt::DirectConnection);
        //启动重连定时器，避免在数据到来时多次调用startConnect()造成重复连接。
        m_reconnectTimer->start();

        m_eventLoop->startMe();
    }

    connect(this,SIGNAL(startConnectTimer()),this,SLOT(startConnect()),Qt::QueuedConnection);
    connect(this,SIGNAL(netConnected(bool)),this,SLOT(respNetConnected(bool)),Qt::QueuedConnection);

    //创建主动连接线程
    m_connectThread = std::move(std::thread(&DataForwardClient::reconnectServer,this));
}

/*!
* @brief 线程连接远程服务器
*/
void DataForwardClient::reconnectServer()
{
    while(m_bConnectorRunningFlag){

        if(!m_tcpClient->connected())
            m_tcpClient->connect();

        std::unique_lock<std::mutex> ul(m_connectMutex);
        m_cv.wait(ul);
    }
}

} //namespace Related{
