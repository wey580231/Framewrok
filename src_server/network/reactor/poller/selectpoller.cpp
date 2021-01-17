#include "selectpoller.h"

#include <QDebug>

namespace Network{

SelectPoller::SelectPoller()
{

}

void SelectPoller::poll(int timeout, ChannelList &activeChannel)
{
    int t_i_maxfd = 0;

    struct timeval tv;
    tv.tv_sec = timeout;
    tv.tv_usec = 0;

    fd_set f_readSet;
    FD_ZERO(&f_readSet);

    fd_set f_writeSet;
    FD_ZERO(&f_writeSet);

    std::for_each(m_monitorList.begin(),m_monitorList.end(),[&](Channel * channel){

        channel->setActiveEvents(0);

        if(channel->isReading()){
            FD_SET(channel->fd(),&f_readSet);
        }

        if(channel->isWriting()){
            FD_SET(channel->fd(),&f_writeSet);
        }

        if(channel->fd() > t_i_maxfd)
            t_i_maxfd = channel->fd();
    });

    int activeSize = select(t_i_maxfd + 1,&f_readSet,&f_writeSet,nullptr,&tv);
    if(activeSize > 0){
        for(int i = 0; i < m_monitorList.size() && activeSize > 0;i++){
            int t_iActiveEvents = 0;
            if(FD_ISSET(m_monitorList.at(i)->fd(),&f_readSet)){
                t_iActiveEvents |= S_POLLIN;
                activeSize--;
            }

            if(FD_ISSET(m_monitorList.at(i)->fd(),&f_writeSet)){
                t_iActiveEvents |= S_POLLOUT;
                activeSize--;
            }

            if(t_iActiveEvents > 0){
                m_monitorList.at(i)->setActiveEvents(t_iActiveEvents);
                activeChannel.push_back(m_monitorList.at(i));
            }
        }
    }
}

} //namespace Network
