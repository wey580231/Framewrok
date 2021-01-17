/*!
 *  @brief     数据转发服务器
 *  @details   1.接收客户端连接，并将数据转发至远程服务器；
 *             2.支持设置多组转发映射关系
 *  @warning   目前只支持单向数据转发
 *  @author    wey
 *  @version   1.0
 *  @date      2019.05.13
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef DATAFORWARDSERVER_H
#define DATAFORWARDSERVER_H

#include <thread>
#include <mutex>
#include <condition_variable>
#include <base/util/rringbuffer.h>
#include <core/protocol/datastruct.h>
#include <network/reactor/sockhead.h>
#include <network/reactor/socketsops.h>
#include <network/reactor/rtcpclient.h>

class RFixedRingBuffer;

namespace Related{

class EventLoop;

class DataForwardServer
{
public:
    DataForwardServer(Network::NetAddress & address);

    void start();

    void setForwardRule(Core::Datastruct::ForwardList & rule);

private:
    void webConnection(const Network::TcpConnectionPtr &conn);
    void webMessage(const Network::TcpConnectionPtr &conn, const char* data, int len);
    void connectedToRemoteServer(const Network::TcpConnectionPtr &m_ptr);

    void sendDataThread(Network::TcpConnectionPtr & conn);

    void connectCloseCallback(const Network::TcpConnectionPtr &conn);
    void clientMenssage(const Network::TcpConnectionPtr &conn, const char *data, int len);

private:
    Core::Datastruct::ForwardList m_listenList;

    Network::NetAddress m_listenAddress;     /*!< 本地监听信息 */

    Network::EventLoop * m_loop;
    Network::RTcpClient * m_tcpClient;       /*!< 转发至远程客户端 */
    Network::TcpConnectionPtr m_remoteConnection;    /*!< 连接至后端处理软件连接 */

	Base::RFixedRingBuffer m_ringbuffer;

    std::thread m_sendThread;
    std::mutex m_sendMutex;
    std::condition_variable m_condition;
    bool m_b_runningFlag;

};

} // namespace Related

#endif // DATAFORWARDSERVER_H
