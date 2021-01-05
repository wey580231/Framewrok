/*!
 *  @brief     新版客户端连接
 *  @details
 *  @author    wey
 *  @version   1.0
 *  @date      2019.04.18
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef RTCPCLIENT_H
#define RTCPCLIENT_H

#include <mutex>
#include "connector.h"
#include "callbacks.h"
#include "sockhead.h"
#include "tcpconnection.h"
#include "../network_global.h"

namespace Network{

class EventLoop;

class NETWORKSHARED_EXPORT RTcpClient
{
public:
    RTcpClient(EventLoop * loop,NetAddress remoteAddress);

    void connect();
    void disconnect();

    void bindLocalIp(NetAddress localAddress);

    bool connected(){return m_connected;}

    //设置回调接口
    void setConnectionCallback(NewConnectionPtrCallback  callback){m_newConnectionCallback = callback;}
    void setMessageCallback(MessageCallback  callback){m_messageCallback = callback;}
    void setCloseCallback(CloseCallback callback){m_closeCallback = callback;}
    void setWriteComplateCallback(WriteCompleteCallback callback){m_writeCompleteCallback = callback;}

private:
    void handleConnectedToServer(int sockfd);
    void removeConnection(const TcpConnectionPtr &t_ptr);
    void connHighWaterMakerCallback(const TcpConnectionPtr &t_ptr, int currCacheLen);

private:
    bool m_b_connected;         /*!< 是否已经连接服务器 */
    EventLoop * m_loop;
    Connector * m_connector;
    TcpConnectionPtr m_connptr;
    NetAddress m_remoteAddress;     /*!< 远程待连接服务器地址 */

    MessageCallback m_messageCallback;
    NewConnectionPtrCallback m_newConnectionCallback;
    CloseCallback m_closeCallback;
    WriteCompleteCallback m_writeCompleteCallback;

    std::mutex m_mutex;
    bool m_connected;       /*!< 是否连接至服务器 */
};

} //namespace Network

#endif // RTCPCLIENT_H
