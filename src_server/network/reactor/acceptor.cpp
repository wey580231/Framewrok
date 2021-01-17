#include "acceptor.h"

#include <QDebug>
#include "socketsops.h"
#include "eventloop.h"

namespace Network{

Acceptor::Acceptor(EventLoop * loop,NetAddress address):
    m_loop(loop),
    m_serversock(Sock::createNonblockingSock())
{
    m_channel = new Channel(loop,m_serversock.fd());
    m_channel->setReadCallback(std::bind(&Acceptor::handleRead,this));

    m_serversock.bind(address);
}

void Acceptor::listen()
{
    m_serversock.listen();
    m_channel->enabelReading();
}

void Acceptor::handleRead()
{
    NetAddress remoteAddress;
    int rfd = m_serversock.accept(remoteAddress);
    if(rfd > 0){
        if(m_connectionCallback)
            m_connectionCallback(rfd,remoteAddress);
        else
            Sock::rclose(rfd);
    }
}

} //namespace Network{
