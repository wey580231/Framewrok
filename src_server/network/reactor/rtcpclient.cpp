#include "rtcpclient.h"

#include <QDebug>
#include "eventloop.h"

namespace Network{

RTcpClient::RTcpClient(EventLoop *loop, NetAddress remoteAddress):
    m_b_connected(false),
    m_loop(loop),m_connector(new Connector(loop,remoteAddress)),m_remoteAddress(remoteAddress),
    m_newConnectionCallback(defaultConnectionCallback),m_closeCallback(defaultCloseCallback),m_connected(false)
{
    m_connector->setNewConnectionCallback(std::bind(&RTcpClient::handleConnectedToServer,this,std::placeholders::_1));
}

void RTcpClient::connect()
{
    m_connector->start();
}

void RTcpClient::disconnect()
{
    m_b_connected = false;

    std::lock_guard<std::mutex> lg(m_mutex);
    if(m_connptr)
        m_connptr->shutdown();
}

/*!
 * @brief 向服务器端连接时，绑定本地ip地址和端口
 * @param[in] localAddress 本地ip信息
 */
void RTcpClient::bindLocalIp(NetAddress localAddress)
{
    m_connector->bindLocalIp(localAddress);
}

/*!
 * @brief 连接服务器成功之后回调
 * @param[in] sockfd 已连接sock描述符
 */
void RTcpClient::handleConnectedToServer(int sockfd)
{
    NetAddress tmp;
    TcpConnectionPtr t_ptr(new TcpConnection(m_loop,sockfd,tmp,m_remoteAddress));
    m_b_connected = true;
    t_ptr->setSendBuff(16*1024*1024);
    t_ptr->setKeepAlive(true);
    t_ptr->setMessageCallback(m_messageCallback);
    t_ptr->setNewConnectionCallback(m_newConnectionCallback);
    t_ptr->setWriteCompleteCallback(m_writeCompleteCallback);
    t_ptr->setCloseCallback(std::bind(&RTcpClient::removeConnection,this,std::placeholders::_1));
    t_ptr->setHighWaterMakerCallback(std::bind(&RTcpClient::connHighWaterMakerCallback,this,std::placeholders::_1,std::placeholders::_2));

    {
        std::lock_guard<std::mutex> lg(m_mutex);
        m_connptr = t_ptr;
        m_connected = true;
//        qDebug()<<"connected to server:"<<m_connptr.get()<<m_connptr.use_count();
    }
    m_connptr->establishConnect();
}

void RTcpClient::removeConnection(const TcpConnectionPtr &t_ptr)
{
    {
        std::lock_guard<std::mutex> lg(m_mutex);
        m_connptr.reset();
        m_connected = false;
        qDebug()<<"removeConnection:"<<m_connptr.get()<<m_connptr.use_count()<<t_ptr.get()<<t_ptr.use_count();
    }

    m_loop->queueInLoop(std::bind(&TcpConnection::prepareDestory,t_ptr));
qDebug()<<"prepareDestory:"<<m_connptr.get()<<m_connptr.use_count()<<t_ptr.get()<<t_ptr.use_count();
    if(m_closeCallback){
        m_closeCallback(t_ptr);
    }
}

void RTcpClient::connHighWaterMakerCallback(const TcpConnectionPtr &t_ptr, int currCacheLen)
{
    if(t_ptr){
        t_ptr->forceClose();
        qDebug()<<"+++++++++++force close===connHighWaterMakerCallback======="<<t_ptr->remoteAddress().ip;
    }
}

} //namespace Network
