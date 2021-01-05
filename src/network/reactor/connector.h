/*!
 *  @brief     负责Tcp客户端连接
 *  @details
 *  @author    wey
 *  @version   1.0
 *  @date      2019.04.19
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef CONNECTOR_H
#define CONNECTOR_H

#include "callbacks.h"
#include "tcpconnection.h"

namespace Network{

class EventLoop;

class Connector
{
public:
    Connector(EventLoop * loop, NetAddress remoteAddress);

    void start();

    void bindLocalIp(NetAddress localAddress);

    void setNewConnectionCallback(NewConnectionCallback callback){m_newConnectiCallback = callback;}

private:
    void connect();
    void startInLoop();
    void conected(int sockfd);
    void retry();

private:
    EventLoop * m_loop;
    NetAddress m_remoteAddress;
    NetAddress m_localAddress;      /*!< 绑定本地的ip地址 */
    ChannelPtr m_channelPtr;

    NewConnectionCallback m_newConnectiCallback;

    bool m_bConnect;        /*!< 是否处于连接过程中 */
};

} //namespace Network

#endif // CONNECTOR_H
