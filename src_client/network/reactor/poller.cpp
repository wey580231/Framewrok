#include "poller.h"

namespace Network{

Poller::Poller()
{

}

void Poller::updateChannel(Channel *channel)
{
    std::lock_guard<std::mutex> lg(m_mutex);
    auto iter = std::find_if(m_monitorList.begin(),m_monitorList.end(),[&](Channel * chan){
        return chan->fd() == channel->fd();
    });
    if(iter == m_monitorList.end())
        m_monitorList.push_back(channel);
}

void Poller::removeChannel(Channel *channel)
{
    std::lock_guard<std::mutex> lg(m_mutex);
    if(m_monitorList.contains(channel))
        m_monitorList.removeOne(channel);
}

} //namespace Network
