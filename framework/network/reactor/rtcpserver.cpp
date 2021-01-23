#include "rtcpserver.h"

#include <QDebug>
#include "acceptor.h"
#include "rtcpclient.h"
#include "tcpconnection.h"
#include "eventloop.h"

namespace Network{

void defaultConnectionCallback(const TcpConnectionPtr & conn){
    qDebug()<<"+++establish";
}

void defaultMessageCallback(const TcpConnectionPtr & conn,const char * recvBuff,int recvLen){
    qDebug()<<conn->fd()<<" recvlen "<<recvLen;
}

void defaultCloseCallback(const TcpConnectionPtr & conn)
{
    qDebug()<<"defaultCloseCallback:"<<conn->fd();
}

RTcpServer::RTcpServer(EventLoop *loop, NetAddress &address):
    m_loop(loop),
    m_acceptor(new Acceptor(loop,address)),
    m_localAddress(address),
    m_messageCallback(defaultMessageCallback),
    m_newConnectionCallback(defaultConnectionCallback),
    m_closeCallback(defaultCloseCallback)
{
    m_acceptor->setConnectionCallback(std::bind(&RTcpServer::handleClientConnect,this,std::placeholders::_1,std::placeholders::_2));
}

void RTcpServer::start()
{
    m_acceptor->listen();
}

void RTcpServer::handleClientConnect(int sockfd, NetAddress remoteaddr)
{
    TcpConnectionPtr ptr(new TcpConnection(m_loop,sockfd,m_localAddress,remoteaddr));

    ptr->setMessageCallback(m_messageCallback);
    ptr->setNewConnectionCallback(m_newConnectionCallback);
    ptr->setCloseCallback(std::bind(&RTcpServer::removeConnection,this,std::placeholders::_1));
    ptr->establishConnect();

    m_connectionList.push_back(ptr);
}

void RTcpServer::removeConnection(const TcpConnectionPtr &ptr)
{
    int size = m_connectionList.removeAll(ptr);
    Q_ASSERT(size == 1);
    if(m_closeCallback)
        m_closeCallback(ptr);

    qDebug()<<m_connectionList.size()<<"+++remove";

    m_loop->queueInLoop(std::bind(&TcpConnection::prepareDestory,ptr));
}

} //namespace Network
