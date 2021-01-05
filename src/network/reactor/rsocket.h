#ifndef SOCKET_H
#define SOCKET_H

#include "sockhead.h"

namespace Network{

class RSocket
{
public:
    RSocket(int sockfd);

    int fd()const{return m_sockfd;}

    int bind(NetAddress & address);
    int listen();
    int accept(NetAddress & remoteAddress);

private:
    int m_sockfd;

};

} //namespace Network

#endif // SOCKET_H
