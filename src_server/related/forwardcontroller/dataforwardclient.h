/*!
 *  @brief     数据转发
 *  @details   将接受的的原始数据通过tcp转发至远程服务器，可设置转发的数据类型；若不在线则提供定时重连
 *  @attention 为了支持断开后重新连接，增加了超时重连定时器，但因为定时器在主线程中创建、在解析线程中对定时器开关、在eventloop中对定时器重启，
 *             涉及到这三处关于跨线程定时器操作，不能直接通过调用方式操作(会出现qt警告)，只可以通过信号槽方式连接；
 *  @author    wey
 *  @version   1.0
 *  @date      2019.07.18
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef DATAFORWARDCLIENT_H
#define DATAFORWARDCLIENT_H

#include <mutex>
#include <condition_variable>
#include <thread>
#include <QList>
#include <network/reactor/sockhead.h>
#include <network/reactor/socketsops.h>
#include <core/protocol/datastruct.h>
#include "core/core_global.h"

class QTimer;

namespace Network{
class EventLoop;
class RTcpClient;
}

namespace Related{

class DataForwardClient : public QObject
{
    Q_OBJECT
public:
    DataForwardClient(Network::NetAddress remoteAddress);
    ~DataForwardClient();

    bool connected(){return m_bIsConnected;}

    void send(char *data,size_t len);
    void sendData(std::list<Core::Datastruct::ProtocolArray> & dataList);

signals:
    void startConnectTimer();
    void netConnected(bool flag);

public slots:
    void startConnect();

private:
    void connectedToRemoteServer(const Network::TcpConnectionPtr &connectedConn);
    void disconnectedFromeServer(const Network::TcpConnectionPtr &connectedConn);

public slots:
    void reconnectTimeout();
    void respNetConnected(bool flag);

private:
    void init();
    void reconnectServer();

private:
    bool m_bIsConnected;            /*!< 是否连接至服务器 */
    QTimer * m_reconnectTimer;      /*!< 超时重连定时器 */
    int m_iTryConnectTimes;         /*!< 尝试连接的次数 */

    Network::NetAddress m_remoteAddress;     /*!< 远程服务器ip地址和端口号信息 */
    Network::EventLoop * m_eventLoop;        /*!< 事件线程 */

    Network::RTcpClient * m_tcpClient;       /*!< 本地客户端 */
    Network::TcpConnectionPtr m_remoteConnection;    /*!< 连接至服务器数据通道连接 */

    bool m_bConnectorRunningFlag;     /*!< 连接线程运行状态 */
    std::mutex m_connectMutex;
    std::condition_variable m_cv;
    std::thread m_connectThread;      /*!< 客户端连接线程 */
};

} //namespace Related

#endif // WRZ_DATAFORWARDCLIENT_H
