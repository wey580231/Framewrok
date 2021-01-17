#include "rsocket.h"

#include <QDebug>
#include "socketsops.h"

namespace Network{

RSocket::RSocket(int sockfd):m_sockfd(sockfd)
{
//    qDebug()<<"socket:"<<sockfd;
}

int RSocket::bind(NetAddress &address)
{
    return Sock::rbind(m_sockfd,address.ip.toLocal8Bit(),address.port);
}

int RSocket::listen()
{
    return Sock::rlisten(m_sockfd);
}

int RSocket::accept(NetAddress &remoteAddress)
{
    sockaddr_in sin;
    int len = sizeof(sockaddr_in);
    int rfd = ::accept(m_sockfd,(sockaddr *)&sin,&len);

    if(rfd > 0){
        remoteAddress.ip = QString(inet_ntoa(sin.sin_addr));
        remoteAddress.port = ntohs(sin.sin_port);
    }

    return rfd;
}

} //namespace Network
