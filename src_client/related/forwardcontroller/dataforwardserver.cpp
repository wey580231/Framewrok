#include "dataforwardserver.h"

#include <functional>
#include <QDebug>
#include <network/reactor/eventloop.h>
#include <network/reactor/rtcpserver.h>
#include <network/reactor/tcpconnection.h>

namespace Related{

static qint64 sendLen = 0;
static qint64 totalLen = 0;

DataForwardServer::DataForwardServer(Network::NetAddress &address):m_listenAddress(address),m_ringbuffer(M_BYTES(50)),m_b_runningFlag(true)
{

}

void DataForwardServer::start()
{
    m_loop = new Network::EventLoop();

    Network::RTcpServer * server = new Network::RTcpServer(m_loop,m_listenAddress);
    server->setNewConnectionCallback(std::bind(&DataForwardServer::webConnection,this,std::placeholders::_1));
    server->setMessageCallback(std::bind(&DataForwardServer::webMessage,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3));
    server->setCloseCallback(std::bind(&DataForwardServer::connectCloseCallback,this,std::placeholders::_1));

    m_tcpClient = new Network::RTcpClient(m_loop,Network::NetAddress("127.0.0.1",8888));
    m_tcpClient->setMessageCallback(std::bind(&DataForwardServer::clientMenssage,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3));
    m_tcpClient->setConnectionCallback(std::bind(&DataForwardServer::connectedToRemoteServer,this,std::placeholders::_1));

    server->start();
    m_loop->startMe();
}

void DataForwardServer::setForwardRule(Core::Datastruct::ForwardList &rule)
{
    m_listenList = rule;
}

void DataForwardServer::connectedToRemoteServer(const Network::TcpConnectionPtr & m_ptr)
{
    m_remoteConnection = m_ptr;
    qDebug()<<"Connected remote server:"<<m_remoteConnection->fd();
    m_b_runningFlag = true;
    m_sendThread = std::move(std::thread(&DataForwardServer::sendDataThread,this,std::ref(m_remoteConnection)));
}

void DataForwardServer::sendDataThread(Network::TcpConnectionPtr &conn)
{
    while(m_b_runningFlag){
        while(m_ringbuffer.dataSize() == 0 && m_b_runningFlag){
            std::unique_lock<std::mutex> lg(m_sendMutex);
            m_condition.wait(lg);
        }
        if(m_b_runningFlag){
            QByteArray t_availableData;
            {
                std::unique_lock<std::mutex> lg(m_sendMutex);
                m_ringbuffer.read(t_availableData,m_ringbuffer.dataSize());
            }
            if(t_availableData.size() > 0){
                totalLen += t_availableData.length();
            }
        }
    }

    qDebug()<<"thread quit~";
}

void DataForwardServer::connectCloseCallback(const Network::TcpConnectionPtr &conn)
{
    if(m_sendThread.joinable()){
        m_b_runningFlag = false;
        m_condition.notify_one();
        m_sendThread.join();
        m_tcpClient->disconnect();
    }
}

void DataForwardServer::webConnection(const Network::TcpConnectionPtr &conn)
{
    qDebug()<<"Device:"<<conn->fd()<<" connected!!!";
    m_tcpClient->connect();
}

void DataForwardServer::webMessage(const Network::TcpConnectionPtr &conn, const char *data, int len)
{
    static qint64 recvDataLen = 0;
    {
        std::unique_lock<std::mutex> lg(m_sendMutex);
        m_ringbuffer.append(data,len);
    }
    recvDataLen += len;

    static int loop = 1;
    if((loop++) % 1000 == 0){
        qDebug("total:%lld sendLen:%lld  diff:%lld",recvDataLen,sendLen,(recvDataLen - sendLen));
        loop = 1;
    }

    m_condition.notify_one();
}

void DataForwardServer::clientMenssage(const Network::TcpConnectionPtr &conn, const char *data, int len)
{
    qDebug()<<"client recv:"<<len<<conn->fd();
}

} //namespace Related
