/*!
 *  @brief     select 多路复用
 *  @details   可在windows、linux下复用
 *  @author    wey
 *  @version   1.0
 *  @date      2019.04.18
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef SELECTPOLLER_H
#define SELECTPOLLER_H

#include "../poller.h"

namespace Network{

class SelectPoller : public Poller
{
public:
    SelectPoller();

    void poll(int timeout, ChannelList &activeChannel) override;

};

} //namespace Network

#endif // SELECTPOLLER_H
