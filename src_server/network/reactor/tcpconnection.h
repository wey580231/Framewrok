/*!
 *  @brief     tcp数据连接链路
 *  @details   1.与channel交互，设置其回调函数
 *  @author    wey
 *  @version   1.0
 *  @date      2019.04.18
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H

#include <memory>
#include <QByteArray>
#include "socketsops.h"
#include "../network_global.h"

namespace Network{

class Channel;
class EventLoop;

class NETWORKSHARED_EXPORT TcpConnection : public std::enable_shared_from_this<TcpConnection>
{
public:
    explicit TcpConnection(EventLoop * loop, int sockfd, NetAddress localAddress, NetAddress remoteAddress);
    ~TcpConnection();

    int fd();
    const NetAddress localAddress(){return m_localAddress;}
    const NetAddress remoteAddress(){return m_remoteAddress;}

    /*!< 由用户调用 */
    void send(const char * data,int len);
    void send(const std::string & message);

    void setSendBuff(int sendBuffLen);
    void setRecvBuff(int recvBuffLen);
    void setKeepAlive(bool enabled, int idleSec = 5, int intervalSec = 3, int tryCount = 3);

    /*!< 由poller根据激活的事件调用 */
    void handleRead();
    void handleWrite();

    void establishConnect();
    void prepareDestory();

    /*!< 数据接收回调 */
    void setMessageCallback(MessageCallback callback){m_messageCallback = callback;}
    /*!< 网络关闭回调 */
    void setCloseCallback(CloseCallback callback){m_closeCallback = callback;}
    /*!< 网路连接回调，可通过TcpConnection 进行数据发送 */
    void setNewConnectionCallback(NewConnectionPtrCallback callback){m_connectionCallback = callback;}
    /*!< 发送缓冲区阻塞过多回调 */
    void setHighWaterMakerCallback(HighWaterMarkCallback callback){m_iHighWaterMarkCallback = callback;}
    /*!< 数据发送完回调 */
    void setWriteCompleteCallback(WriteCompleteCallback callback){m_writeCompleteCallback = callback;}

    void shutdown();

    void forceClose();
    void forceCloseInLoop();

private:
    enum ConnectionState{RConnecting,RConnected,RDisconnecting,RDisconnected};

    void sendInLoop(const std::string & message);
    void sendInLoop(const char * data, int len);

    void handleClose();
    void shutdownInLoop();

    void setState(ConnectionState newState){m_state = newState;}

private:
    int m_i_sockfd;                 /*!< socket句柄 */
    QString m_uuid;
    EventLoop * m_loop;
    ChannelPtr m_channelPtr;
    MessageCallback m_messageCallback;
    CloseCallback m_closeCallback;
    NewConnectionPtrCallback m_connectionCallback;
    WriteCompleteCallback m_writeCompleteCallback;
    HighWaterMarkCallback m_iHighWaterMarkCallback;

    NetAddress m_localAddress;
    NetAddress m_remoteAddress;

    QByteArray m_sendBuff;

    ConnectionState m_state;

    int m_iHighWaterMark;       /*!< 发送缓冲区高水位预警，默认为64M */
};

} //namespace Network

#endif // TCPCONNECTION_H
