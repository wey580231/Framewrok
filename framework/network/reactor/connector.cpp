#include "connector.h"

#include <future>

#include <QDebug>

#include "socketsops.h"
#include "channel.h"
#include "eventloop.h"

namespace Network{

Connector::Connector(EventLoop *loop, NetAddress remoteAddress):
    m_loop(loop),m_remoteAddress(remoteAddress),m_bConnect(false)
{

}

void Connector::start()
{
    m_bConnect = true;
    m_loop->runInLoop(std::bind(&Connector::startInLoop,this));
}

/*!
 * @brief 绑定指定ip进行连接
 * @param[in] localAddress 本地ip及网卡
 */
void Connector::bindLocalIp(NetAddress localAddress)
{
    m_localAddress = localAddress;
}

void Connector::startInLoop()
{
    if(m_bConnect){
        connect();
    }else{
        qDebug()<<"do not connect ";
    }
}

void Connector::connect()
{
    int fd = Sock::createNonblockingSock();

    if(!m_localAddress.ip.isEmpty()){
        Sock::setReuseAddr(fd,true);
        qDebug()<<"==Connector:connect:bind:"<<Sock::rbind(fd,m_localAddress.ip.toLocal8Bit().data(),m_localAddress.port);
    }

    int result = Sock::rconnect(fd,m_remoteAddress.ip.toLocal8Bit().data(),m_remoteAddress.port);
    switch(result){
        case 0:
        case EINPROGRESS:
        case EINTR:
        case EISCONN:
            conected(fd);
            break;

        case EAGAIN:
        case EADDRINUSE:
        case EADDRNOTAVAIL:
        case ECONNREFUSED:
        case ENETUNREACH:
            retry();
            break;

        case EFAULT:
        case EBADF:
        case ENOTSOCK:
            Sock::rclose(fd);
            break;

        default:
            Sock::rclose(fd);
            break;
    }
}

/*!
 * @brief 连接已经建立
 * @param[in]   sockfd 建立连接socket描述符
 */
void Connector::conected(int sockfd)
{
    m_channelPtr.reset(new Channel(m_loop,sockfd));
//    qDebug()<<"Connector:connected to host:"<<sockfd<<m_remoteAddress.ip;
    if(m_newConnectiCallback)
        m_newConnectiCallback(sockfd);
}

/*!
 * @brief 连接未建立，3秒后重试
 */
void Connector::retry()
{

}

} //namespace Network
