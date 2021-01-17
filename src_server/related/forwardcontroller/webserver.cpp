#include "webserver.h"

#include <functional>
#include <QDebug>
#include <core/file/programfilepath.h>
#include <network/reactor/eventloop.h>
#include <network/reactor/rtcpserver.h>
#include <network/reactor/tcpconnection.h>

namespace Related{

WebServer::WebServer(Network::NetAddress &address):m_listenAddress(address)
{

}

void WebServer::start()
{
    Network::EventLoop * loop = new Network::EventLoop();

    Network::RTcpServer * server = new Network::RTcpServer(loop,m_listenAddress);
    server->setNewConnectionCallback(std::bind(&WebServer::webConnection,this,std::placeholders::_1));
    server->setMessageCallback(std::bind(&WebServer::webMessage,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3));
    server->start();
    loop->startMe();
}

/*!
 * @brief 新web客户端连接
 * @param[in] conn 连接指针
 */
void WebServer::webConnection(const Network::TcpConnectionPtr & conn){
    qDebug()<<"web:"<<conn->fd();
}

/*!
 * @brief 接收web客户端数据
 * @param[in]   conn 网络链路连接
 * @param[in]   data 网络数据
 * @param[in]   len 数据长度
 */
void WebServer::webMessage(const Network::TcpConnectionPtr & conn,const char* data,int len){
    static int recvlen = 0;
    recvlen += len;
    qDebug()<<"webMessage recv:"<<conn->fd()<<len<<recvlen;
}


} //namespace Related
