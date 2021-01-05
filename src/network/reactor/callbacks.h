/*!
 *  @brief     数据回调接口定义
 *  @details   定义若干回调接口函数
 *  @author    wey
 *  @version   1.0
 *  @date      2019.04.18
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef CALLBACKS_H
#define CALLBACKS_H

#include <memory>
#include <functional>
#include <QObject>

#ifdef Q_OS_WIN
#include <winsock2.h>
#include <windows.h>
#include <mstcpip.h>
#endif

#include "sockhead.h"

namespace Network{

typedef std::function<void()> Functor;

class TcpConnection;
typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;

class Channel;
typedef std::shared_ptr<Channel> ChannelPtr;

const int g_i_tcpRecvBuffLen = 64*1024;

typedef std::function<void(int)> NewConnectionCallback;
typedef std::function<void(const TcpConnectionPtr&)> NewConnectionPtrCallback;
typedef std::function<void(int,NetAddress &)> ConnectionCallback;
typedef std::function<void()> ReadCallback;
typedef std::function<void()> WriteCallback;
typedef std::function<void(const TcpConnectionPtr&)> CloseCallback;
typedef std::function<void(const TcpConnectionPtr &,const char*,int)> MessageCallback;
typedef std::function<void(const TcpConnectionPtr&)> WriteCompleteCallback;
typedef std::function<void(const TcpConnectionPtr &,int)> HighWaterMarkCallback;

void defaultConnectionCallback(const TcpConnectionPtr & conn);
void defaultMessageCallback(const TcpConnectionPtr & conn,const char * recvBuff,int recvLen);
void defaultCloseCallback(const TcpConnectionPtr & conn);

} //namespace Network

#endif // CALLBACKS_H
