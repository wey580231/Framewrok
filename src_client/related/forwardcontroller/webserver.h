/*!
 *  @brief     web接收服务器
 *  @details   接收web客户端tcp客户端请求，将接收数据以udp发送至设备.
 *  @author    wey
 *  @version   1.0
 *  @date      2019.05.13
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <network/reactor/sockhead.h>
#include <network/reactor/socketsops.h>


namespace Related{

class WebServer
{
public:
    WebServer(Network::NetAddress & address);

    void start();

private:
    void webConnection(const Network::TcpConnectionPtr &conn);
    void webMessage(const Network::TcpConnectionPtr &conn, const char* data, int len);

private:
    Network::NetAddress m_listenAddress;     /*!< 本地监听信息 */
};

} //namespace Related

#endif // WEBSERVER_H
