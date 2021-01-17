#include "channel.h"

#include <QDebug>
#include "eventloop.h"

namespace Network{

Channel::Channel(EventLoop *loop, int sockId):m_loop(loop),m_i_sockFd(sockId),m_registEvents(0),m_activeEvents(0)
{

}

Channel::~Channel()
{
    qDebug()<<"release channel：sockId:"<<m_i_sockFd;
}

int Channel::fd()
{
    return m_i_sockFd;
}

/*!
 * @brief 处理激活的事件
 * @details 使用select时需要自身维护可读、可写、错误等状态
 */
void Channel::handleEvent()
{
    if(m_activeEvents & S_POLLIN){
        if(m_readCallback)
            m_readCallback();
    }

    if(m_activeEvents & S_POLLOUT){
        if(m_writeCallback)
            m_writeCallback();
    }
}

void Channel::updateStatus()
{
    m_loop->updateChannel(this);
}

void Channel::remove()
{
    m_loop->removeChannel(this);
}

} //namespace Network
