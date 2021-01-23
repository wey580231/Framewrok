/*!
 *  @brief     新版tcp服务器
 *  @details
 *  @author    wey
 *  @version   1.0
 *  @date      2019.04.18
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef RTCPSERVER_H
#define RTCPSERVER_H

#include <QList>
#include "sockhead.h"
#include "callbacks.h"
#include "../network_global.h"

namespace Network{

class EventLoop;
class Acceptor;
class TcpConnection;

class NETWORKSHARED_EXPORT RTcpServer
{
public:
    RTcpServer(EventLoop * loop,NetAddress & address);

    void start();
    void handleClientConnect(int sockfd,NetAddress remoteaddr);

    void removeConnection(const TcpConnectionPtr &ptr);

    /*!< 新客户端连接回调 */
    void setNewConnectionCallback(NewConnectionPtrCallback callback){m_newConnectionCallback = callback;}
    void setMessageCallback(MessageCallback callback){m_messageCallback = callback;}
    void setCloseCallback(CloseCallback callback){m_closeCallback = callback;}

private:
    EventLoop * m_loop;
    Acceptor * m_acceptor;
    NetAddress m_localAddress;
    QList<TcpConnectionPtr> m_connectionList;

    ConnectionCallback m_connectCallback;
    NewConnectionPtrCallback m_newConnectionCallback;
    MessageCallback m_messageCallback;
    CloseCallback m_closeCallback;
};

} //namespace Network

#endif // RTCPSERVER_H
