/*!
 *  @brief     IO多路复用基类
 *  @details   在windows上支持select，在linux下可作为poll、epoll的扩展；
 *             其子类实现poll()方法，对激活的描述符进行监视
 *  @author    wey
 *  @version   1.0
 *  @date      2019.04.18
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef POLLER_H
#define POLLER_H

#include <mutex>
#include "channel.h"

namespace Network{

class Channel;

class Poller
{
public:
    Poller();

    /*!
     * @brief 监视所有描述符，返回激活的描述符
     * @attention 此方法返回时有两种可能，1是在指定超时时间内有激活的描述符；2.时间超时后返回
     * @param[in] timeout 监视最大延时时间，单位为秒
     * @param[in] activeChannel 活动的描述符集合
     */
    virtual void poll(int timeout,ChannelList & activeChannel) = 0;

    void updateChannel(Channel * channel);
    void removeChannel(Channel * channel);

protected:
    ChannelList m_monitorList;      /*!< 待监控channel集合 */
    std::mutex m_mutex;
};

} //namespace Network

#endif // POLLER_H
