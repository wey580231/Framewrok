/*!
 *  @brief     多客户端数据转发
 *  @details   支持将数据转发至多个服务器,可批量设置需要转发的服务器信息。多客户端内部会自动管理每个连接的状态、重连等
 *  @author    wey
 *  @version   1.0
 *  @date      2019.12.24
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef MULITDATAFORWARDCLIENT_H
#define MULITDATAFORWARDCLIENT_H

#include <mutex>
#include <condition_variable>
#include <thread>
#include <QList>
#include <QSharedPointer>

#include <base/util/rringbuffer.h>
#include <core/protocol/datastruct.h>
#include <core/core_global.h>
#include <network/reactor/sockhead.h>
#include <network/reactor/socketsops.h>


class QTimer;

namespace Network{
class EventLoop;
class RTcpClient;
}

namespace Related{

typedef std::function<void(Network::TcpConnectionPtr, Base::RFixedRingBuffer&)> MessageParseCallback;

class SingleForwardClient : public QObject
{
    Q_OBJECT
public:
    SingleForwardClient(Network::EventLoop * eventLoop,Network::NetAddress remoteAddress);
    ~SingleForwardClient();

    bool connected(){return m_bIsConnected;}

    void send(const char *data, size_t len);
    void sendData(std::list<Core::Datastruct::ProtocolArray> & dataList);

    void setDataParse(MessageParseCallback callback){m_dataParse = callback;}

signals:
    void startConnectTimer();
    void netConnected(bool flag);

public slots:
    void startConnect();

private:
    void connectedToRemoteServer(const Network::TcpConnectionPtr &connectedConn);
    void disconnectedFromServer(const Network::TcpConnectionPtr &connectedConn);
    void recvMessageFromServer(const Network::TcpConnectionPtr & connectedConn,const char* data,int dataLen);

    void init();
    void reconnectServer();

public slots:
    void reconnectTimeout();
    void respNetConnected(bool flag);

private:
    bool m_bIsConnected;            /*!< 是否连接至服务器 */
    QTimer * m_reconnectTimer;      /*!< 超时重连定时器 */
    int m_iTryConnectTimes;         /*!< 尝试连接的次数 */

    Network::NetAddress m_remoteAddress;     /*!< 远程服务器ip地址和端口号信息 */

    Network::RTcpClient * m_tcpClient;       /*!< 本地客户端 */
    Network::TcpConnectionPtr m_remoteConnection;    /*!< 连接至服务器数据通道连接 */

    bool m_bConnectorRunningFlag;     /*!< 连接线程运行状态 */
    std::mutex m_connectMutex;
    std::condition_variable m_cv;
    std::thread m_connectThread;      /*!< 客户端连接线程 */

	Base::RFixedRingBuffer m_recvBuffer;    /*!< 接收缓冲区 */
    MessageParseCallback m_dataParse;

    Network::EventLoop * m_eventLoop;        /*!< 事件线程:不拥有所有权 */

    std::mutex m_connectionMutex;     /*!< 网络连接互斥mutex */
};

class MultiForwardClients;

class ForwardClient : public QObject{
    Q_OBJECT
public:
    ForwardClient():m_client(nullptr),m_bHasNotifyConnectServer(false){}

    void startConnect(){
        if(!m_bHasNotifyConnectServer){
            m_bHasNotifyConnectServer = true;

            if(m_client){
                m_client->startConnect();
            }else{
                qDebug()<<"call setRemoteAddress first!";
            }
        }
    }

private slots:
    void respNetConnected(bool connected){
        if(!connected)
            m_bHasNotifyConnectServer = false;
    }

private:
    SingleForwardClient * m_client;               /*!< 单个数据转发客户端 */
    bool m_bHasNotifyConnectServer;               /*!< 在断网状态下已经通知客户端连接服务器，避免多次发送信号 */

    friend class MultiForwardClients;
};

/*!
 *  @brief 多连接客户端
 *  @details 此客户端对外提供操作接口
 */
class MultiForwardClients : public QObject
{
    Q_OBJECT
public:
    explicit MultiForwardClients(Network::EventLoop * eventloop);
    ~MultiForwardClients();

    //网络状态
    bool anyConnected();
    void startConnect();

    void setServerInfo(QList<Network::NetAddress> && serverAddressInfos);

    //数据发送
    void sendData(const char * data,int length);

    //数据解析
    void setDataParse(MessageParseCallback callback){m_defaultParse = callback;}

private:
    void init();

private:
    std::once_flag m_onceForward;

    Network::EventLoop * m_eventLoop;        /*!< 事件线程 */

    QList<Network::NetAddress> m_forwardServerList;     /*!< 待转发服务器地址清单 */
    typedef QSharedPointer<ForwardClient> ClinetPtr;
    QList<ClinetPtr> m_forwardClients;       /*!< 多客户端连接 */

    MessageParseCallback m_defaultParse;

};

} //namespace Related

#endif // MULITDATAFORWARDCLIENT_H
