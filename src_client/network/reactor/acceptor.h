/*!
 *  @brief     TcpServer连接请求接收器
 *  @details   依赖于TcpServer，作为TcpServer和Channel之间连接的纽带。
 *  @author    wey
 *  @version   1.0
 *  @date      2019.04.18
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include "channel.h"
#include "sockhead.h"
#include "rsocket.h"

namespace Network{

class EventLoop;

class Acceptor
{
public:
    Acceptor(EventLoop * loop,NetAddress address);

    void listen();
    void handleRead();

    void setConnectionCallback(ConnectionCallback callback){m_connectionCallback = callback;}

private:
    EventLoop * m_loop;
    Channel * m_channel;
    RSocket m_serversock;

    ConnectionCallback m_connectionCallback;
};

} //namespace Network

#endif // ACCEPTOR_H
