#include "tcpconnection.h"

#include <QDebug>
#include <base/util/rutil.h>
#include "channel.h"
#include "eventloop.h"

namespace Network{

TcpConnection::TcpConnection(EventLoop *loop, int sockfd, NetAddress localAddress, NetAddress remoteAddress):
    m_loop(loop),m_i_sockfd(sockfd),
    m_localAddress(localAddress),m_remoteAddress(remoteAddress),
    m_channelPtr(new Channel(loop,sockfd)),
    m_state(RDisconnecting),
    m_uuid(Base::RUtil::UUID()),
    m_iHighWaterMark(32*1024*1024)
{
    m_channelPtr->setReadCallback(std::bind(&TcpConnection::handleRead,this));
    m_channelPtr->setWriteCallback(std::bind(&TcpConnection::handleWrite,this));
}

TcpConnection::~TcpConnection()
{
    qDebug()<<"release tcpconcection:sockId:"<<m_i_sockfd;
}

int TcpConnection::fd()
{
    return m_i_sockfd;
}

void TcpConnection::send(const char *data, int len)
{
    send(std::string(data,len));
}

void TcpConnection::send(const std::string &message)
{
    if(m_state == RConnected){
        if(m_loop->isInLoopThread()){
            sendInLoop(message);
        }else{
            void (TcpConnection::*fp)(const std::string &message) = &TcpConnection::sendInLoop;
            m_loop->runInLoop(std::bind(fp,shared_from_this(),message));
        }
    }
}

void TcpConnection::sendInLoop(const std::string &message)
{
    sendInLoop(message.data(),message.size());
}

void TcpConnection::sendInLoop(const char *data, int len)
{
    if(m_state == RDisconnected){
        qDebug()<<"tcp connection disconnected~~";
        return;
    }

    int t_iRemainLen = len;
    int t_sendLen = 0;
    bool t_bSockError = false;

    if(!m_channelPtr->writeAble() && m_sendBuff.size() == 0){
        t_sendLen = Sock::rsend(m_i_sockfd,data,len);

        if(t_sendLen > 0){
            t_iRemainLen = len - t_sendLen;

            if(t_iRemainLen == 0 && m_writeCompleteCallback){
                m_loop->queueInLoop(std::bind(m_writeCompleteCallback,shared_from_this()));
            }
        }else{
            int errorCode = GetLastError();
            if(t_sendLen == 0 || (errorCode != WSAETIMEDOUT && errorCode != WSAEWOULDBLOCK)){
                t_bSockError = true;
                handleClose();
            }else{
                qDebug()<<"errorCode:"<<errorCode;
            }
        }
    }

    if(!t_bSockError && t_iRemainLen > 0){
        m_sendBuff.append(data + t_sendLen,len - t_sendLen);

        if(m_sendBuff.size() > m_iHighWaterMark && m_iHighWaterMarkCallback){
            m_loop->queueInLoop(std::bind(m_iHighWaterMarkCallback,shared_from_this(),m_sendBuff.size()));
        }

        if(!m_channelPtr->isWriting()){
            m_channelPtr->enableWriting();
        }

//        qDebug()<<"append data:"<<m_sendBuff.size()<<(len - t_sendLen)<<len<<t_sendLen<<m_localAddress.ip<<m_localAddress.port<<m_remoteAddress.ip;
    }
}

void TcpConnection::handleRead()
{
    char buff[g_i_tcpRecvBuffLen] = {0};

    int recvLen = Sock::rrecv(m_channelPtr->fd(),buff,g_i_tcpRecvBuffLen);
    if(recvLen > 0){
        m_messageCallback(shared_from_this(),buff,recvLen);
    }else if(recvLen == 0){
        handleClose();
    }else{
        int errorCode = GetLastError();
        if(errorCode != WSAETIMEDOUT && errorCode != WSAEWOULDBLOCK){
            handleClose();
        }
        qDebug()<<"error+++";
    }
}

void TcpConnection::handleWrite()
{
    if(m_channelPtr && m_channelPtr->writeAble()){
        int t_sendLen = Sock::rsend(m_i_sockfd,m_sendBuff.data(),m_sendBuff.size());
        if(t_sendLen > 0){
            m_sendBuff.remove(0,t_sendLen);

            if(m_sendBuff.size() == 0){
                m_channelPtr->disableWriting();
            }

            if(m_state == RDisconnecting){
                shutdownInLoop();
            }

            qDebug()<<"=====================left buffer:"<<m_sendBuff.size()<<t_sendLen;
        }else{
            qDebug()<<"write error!";
        }
    }
}

/*!
 * @brief 网络连接已经建立
 */
void TcpConnection::establishConnect()
{
    m_channelPtr->enabelReading();
    setState(RConnected);
    m_connectionCallback(shared_from_this());
}

void TcpConnection::prepareDestory()
{
    if(m_state == RConnected){
        setState(RDisconnected);
        m_channelPtr->disableAll();
    }
    m_channelPtr->remove();
}

void TcpConnection::setSendBuff(int sendBuffLen)
{
    Sock::setSendBuff(m_i_sockfd,sendBuffLen);
}

void TcpConnection::setRecvBuff(int recvBuffLen)
{
    Sock::setRecvBuff(m_i_sockfd,recvBuffLen);
}

void TcpConnection::setKeepAlive(bool enabled, int idleSec, int intervalSec, int tryCount)
{
    Sock::setKeepAlive(m_i_sockfd,enabled,idleSec,intervalSec,tryCount);
}

void TcpConnection::shutdown()
{
    if(m_state == RConnected){
        setState(RDisconnecting);
        m_loop->runInLoop(std::bind(&TcpConnection::shutdownInLoop,shared_from_this()));
    }
}

void TcpConnection::shutdownInLoop()
{
    if(!m_channelPtr->isWriting()){
        Sock::rshutdownWrite(m_i_sockfd);
    }
}

void TcpConnection::forceClose()
{
    if(m_state == RConnected || m_state == RDisconnecting){
        setState(RDisconnecting);
        m_loop->queueInLoop(std::bind(&TcpConnection::forceCloseInLoop,shared_from_this()));
    }
}

void TcpConnection::forceCloseInLoop()
{
    if(m_state == RConnected || m_state == RDisconnecting){
        handleClose();
    }
}

void TcpConnection::handleClose()
{
    m_channelPtr->disableAll();
qDebug()<<"+handleclose:"<<m_i_sockfd<<m_remoteAddress.ip<<m_remoteAddress.port;
    Sock::rclose(m_i_sockfd);

    setState(RDisconnected);

    if(m_closeCallback){
        m_closeCallback(shared_from_this());
    }
}

} //namespace Network
