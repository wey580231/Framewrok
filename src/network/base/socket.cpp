#include "socket.h"

#include <QtGlobal>

#ifdef Q_OS_WIN
#include <winsock2.h>
#include <windows.h>
#include <WS2tcpip.h>
#include <mstcpip.h>
typedef  int socklen_t;
#pragma  comment(lib,"ws2_32.lib")
#elif defined(Q_OS_LINUX)
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>
#define closesocket close
#endif

#include <base/util/rlog.h>

namespace Network {

RSocket::RSocket():socktype(R_NONE)
{
    sockFd = 0;
    socketPort = 0;
    errorCode = 0;
    socketValid = false;
    blockAble = false;
    memset(socketIp,0,sizeof(socketIp));
}

RSocket::RSocket(const RSocket &rsock)
{
    this->sockFd = rsock.sockFd;
    this->socketPort = rsock.socketPort;
    this->errorCode = rsock.errorCode;
    this->socketValid = rsock.socketValid;
    this->blockAble = rsock.blockAble;
    memset(socketIp,0,sizeof(socketIp));
    memcpy(this->socketIp,rsock.socketIp,sizeof(socketIp));
}

bool RSocket::createSocket(SocketType socktype)
{
#ifdef Q_OS_WIN
    static bool isInit = false;
    if(!isInit)
    {
        isInit = true;
        WSADATA ws;
        if(WSAStartup(MAKEWORD(2,2),&ws) != 0)
        {
            RLOG_ERROR("Init windows socket failed!");
            return false;
        }
    }
#endif

    sockFd = socket(AF_INET,socktype,0);
    if(sockFd == INVALID_SOCKET)
    {
        errorCode = getErrorCode();
        RLOG_ERROR("Create socket failed! [ErrorCode:%d]",errorCode);
        return false;
    }

    socketValid = true;
    return true;
}

bool RSocket::bind(const char *ip, unsigned short port)
{
    if(!isValid())
        return false;

    localAddr.sin_family = AF_INET;
    localAddr.sin_port = htons(port);
//    localAddr.sin_addr.s_addr = inet_addr(ip);
    localAddr.sin_addr.s_addr = htonl(INADDR_ANY)/*inet_addr(ip)*/;

    int ret = ::bind(sockFd,(sockaddr*)&localAddr,sizeof(localAddr));
    if(ret == SOCKET_ERROR)
    {
        closeSocket();
        errorCode = getErrorCode();
        RLOG_ERROR("Bind socket error [%s:%d] [ErrorCode:%d]",ip,port,errorCode);
        return false;
    }

    strcpy(socketIp,ip);
    socketPort = port;

    return true;
}

/*!
 * @brief 开启socket监听
 * @param[in] backlog 同一时刻最大允许连接数量
 * @return 是否监听成功
 */
bool RSocket::listen(int backlog)
{
    if(!isValid())
        return false;

    int ret = ::listen(sockFd,backlog);
    if(ret == SOCKET_ERROR)
    {
        closeSocket();
        errorCode = getErrorCode();
        RLOG_ERROR("Listen socket error! [ErrorCode:%d]",errorCode);
        return false;
    }

    RLOG_INFO("Listen socket success,[%s:%d]!",socketIp,socketPort);

    return true;
}

bool RSocket::closeSocket()
{
    if(isValid())
    {
        if(closesocket(sockFd) == 0)
        {
            socketValid = false;
            sockFd = 0;
            return true;
        }
#ifdef Q_OS_WIN
        RLOG_ERROR("Close socket error [ErrorCode:%d]!",GetLastError());
#endif
    }
    return false;
}

/*!
 * @brief 作为server端接收客户端连接
 * @param[in] 无
 * @return 返回接收的socket描述信息
 */
RSocket RSocket::accept()
{
    RSocket tmpSocket;
    if(!isValid())
        return tmpSocket;

    sockaddr_in clientAddr;
    int len = sizeof(clientAddr);

    int clientSocket = ::accept(sockFd,(sockaddr*)&clientAddr,(socklen_t*)&len);
    if(clientSocket == INVALID_SOCKET)
    {
        RLOG_ERROR("Accept failed [ErrorCode:%d]!",GetLastError());
        return tmpSocket;
    }

    strcpy(tmpSocket.socketIp,inet_ntoa(clientAddr.sin_addr));
    tmpSocket.socketValid = true;
    tmpSocket.socketPort= ntohs(clientAddr.sin_port);
    tmpSocket.sockFd = clientSocket;
    tmpSocket.socktype = R_TCP;

    RLOG_INFO("Recv socket [%s:%d]",tmpSocket.socketIp,tmpSocket.socketPort);

    return tmpSocket;
}

/*!
 * @brief 接收数据，并将结果保存至缓冲区
 * @param[out] buff 保存并返回接收的数据
 * @param[in] length 缓冲区数据长度
 * @return 实际接收数据的长度
 */
int RSocket::recv(char *buff, int length)
{
    if(!isValid() || buff == NULL)
        return -1;

    size_t buffLen = strlen(buff);
    memset(buff,0,buffLen);

    int ret = ::recv(sockFd,buff,length,0);

    if(ret <= 0){
        errorCode = getErrorCode();
    }

    return ret;
}

/*!
 * @brief 发送一定长度数据
 * @param[in] buff 待发送数据的缓冲区
 * @param[in] length 待发送的长度
 * @return 是否发送成功
 */
int RSocket::send(const char *buff, const int length)
{
    if(!isValid() || buff == NULL)
        return -1;

    int sendLen = 0;
    while(sendLen != length)
    {
        int ret = ::send(sockFd,buff+sendLen,length-sendLen,0);
        if (ret <= 0)
        {
            sendLen = -1;
            break;
        }
        sendLen += ret;
    }
    return sendLen;
}

/*!
 * @brief 连接socket
 * @param[in] remoteIp 远程IP
 * @param[in] remotePort 远程端口
 * @param[in] teimeouts 超时时间
 * @return 连接是否成功
 */
bool RSocket::connect(const char *remoteIp, const unsigned short remotePort, int timeouts)
{
    if(!isValid())
        return false;

    sockaddr_in remoteAddr;
    remoteAddr.sin_family = AF_INET;
    remoteAddr.sin_port = htons(remotePort);
    remoteAddr.sin_addr.s_addr = inet_addr(remoteIp);

    setBlock(false);

    if(::connect(sockFd,(sockaddr*)&remoteAddr,sizeof(remoteAddr)) != 0)
    {
        fd_set set;
        FD_ZERO(&set);
        FD_SET(sockFd,&set);
        timeval tm;
        tm.tv_sec = timeouts;
        tm.tv_usec = 0;
        if(select(sockFd+1,0,&set,0,&tm) <= 0)
        {
            RLOG_ERROR("Connect timeout or error [%s:%d] %s ",remoteIp,remotePort,strerror(errno));
            return false;
        }
    }

    setBlock(true);
    RLOG_INFO("connect %s:%d success!\n",remoteIp,remotePort);
    return true;
}

/*!
 * @brief 设置socket是否为阻塞模式
 * @param[in] flag 状态
 * @return 返回设置的是否成功设置
 */
bool RSocket::setBlock(bool flag)
{
    if(!isValid())
        return false;

#if defined(Q_OS_WIN)
    unsigned long ul = 0;
    if(!flag)
        ul = 1;

    ioctlsocket(sockFd,FIONBIO,&ul);
#else defined(Q_OS_LINUX)
    int flags = fcntl(sockFd,F_GETFL,0);
    if(flags<0)
        return false;

    if(flag)
        flags = flags&~O_NONBLOCK;
    else
        flags = flags|O_NONBLOCK;

    if(fcntl(sockFd,F_SETFL,flags)!=0)
        return false;
#endif
    return true;
}

/*!
 * @brief UDP数据接收
 * @param[in] buff  char * 接收的数据缓冲区
 * @param[in] length  int  接收的数据长度
 * @return 实际接收数据的长度
 */
int RSocket::recvFrom(char *buff, int length,std::string & remoteIp,ushort & remotePort)
{
    if(!isValid())
        return -1;

    int fromlen = sizeof(sockaddr);
    int recvLen = ::recvfrom(sockFd,buff,length,0,(sockaddr *)&localAddr,&fromlen);
    if(recvLen < 0){
        errorCode = getErrorCode();
        if(errorCode != WSAETIMEDOUT && errorCode != WSAEWOULDBLOCK){
#ifdef Q_OS_WIN
        RLOG_ERROR("Recv socket error [ErrorCode:%d]!",GetLastError());
#endif
        }
        return -1;
    }

    remoteIp.clear();
    remoteIp.append(inet_ntoa(localAddr.sin_addr));
    remotePort = ntohs(localAddr.sin_port);

    return recvLen;
}

/*!
 * @brief UDP数据发送
 * @param[in] buff char* 待发送数据缓冲区
 * @param[in] length int 待发送数据长度
 * @param[in] dest char* 目的地址
 * @param[in] port int   目的端口号
 * @return 实际发送数据长度
 */
int RSocket::sendTo(const char *buff, const int length, const char *dest, const int port)
{
    if(!isValid())
        return -1;

    sockaddr_in raddr;
    raddr.sin_family = AF_INET;
    raddr.sin_port = htons(port);
    raddr.sin_addr.s_addr = inet_addr(dest);

    int sendLen = 0;
    while(sendLen != length)
    {
        int ret = ::sendto(sockFd,buff,length,0,(sockaddr*)&raddr,sizeof(sockaddr_in));
        if (ret <= 0)
        {
            sendLen = -1;
            break;
        }
        sendLen += ret;
    }

    return sendLen;
}

/*!
 * @brief 加入组播网络
 * @param[in]   groupIp 组播IP地址
 * @return 是否加入成功
 */
bool RSocket::joinGroup(const char *groupIp)
{
    if(!isValid())
        return false;

    if(groupIp == nullptr || socketIp == nullptr )
        return false;

    struct ip_mreq multiCast;
    memset((char *)&multiCast,0,sizeof(ip_mreq));

    multiCast.imr_multiaddr.s_addr = inet_addr(groupIp);
    multiCast.imr_interface.s_addr = inet_addr(socketIp);

    int ret = setsockopt(sockFd,IPPROTO_IP,IP_ADD_MEMBERSHIP,(char *)&multiCast,sizeof(ip_mreq));
    if(ret != 0){
        errorCode = GetLastError();
        RLOG_ERROR("Join multicast[%s] error[%d] !",groupIp,errorCode);
        return false;
    }
    return true;
}

int RSocket::setSendBuff(int bytes)
{
   return setSockopt(SO_SNDBUF,(char *)&bytes,sizeof(bytes));
}

int RSocket::setRecvBuff(int bytes)
{
    return setSockopt(SO_RCVBUF,(char *)&bytes,sizeof(bytes));
}

int RSocket::setRecvTimeOut(int millsecond)
{
    return setSockopt(SO_RCVTIMEO,(char *)&millsecond,sizeof(millsecond));
}

int RSocket::setSendTimeOut(int millsecond)
{
    return setSockopt(SO_SNDTIMEO,(char *)&millsecond,sizeof(millsecond));
}

int RSocket::setSockopt(int optname, const char *optval, int optlen)
{
    if(!isValid())
        return -1;

    return setsockopt(sockFd,SOL_SOCKET,optname,optval,optlen);
}

int RSocket::getSockopt(int optname, char *optval, int optlen)
{
    if(!isValid())
        return -1;

    return getsockopt(sockFd,SOL_SOCKET,optname,optval,&optlen);
}

int RSocket::getSendBuff()
{
    int sendBuff = 0;
    int result = getSockopt(SO_SNDBUF,(char *)&sendBuff,sizeof(sendBuff));
    if(result < 0)
        return -1;
    return sendBuff;
}

int RSocket::getRecvBuff()
{
    int sendBuff = 0;
    int result = getSockopt(SO_RCVBUF,(char *)&sendBuff,sizeof(sendBuff));
    if(result < 0)
        return -1;
    return sendBuff;
}

int RSocket::getSendTimeout()
{
    int sendBuff = 0;
    int result = getSockopt(SO_SNDTIMEO,(char *)&sendBuff,sizeof(sendBuff));
    if(result < 0)
        return -1;
    return sendBuff;
}

int RSocket::getRecvTimeOut()
{
    int sendBuff = 0;
    int result = getSockopt(SO_RCVTIMEO,(char *)&sendBuff,sizeof(sendBuff));
    if(result < 0)
        return -1;
    return sendBuff;
}

/*!
* @brief 是否开启KeepAlive检测
* @details 为了检测链路是否正常，特别是网线突然断掉特殊情况下。由系统主动发送探测包，用来检测链路是否
* @param[in] enabled true时开启、false是关闭
* @param[in] idle 出发检测的时间间隔(此时间内没有数据来往),单位s
* @param[in] interval 探测包发出的时间间隔，单位s
* @param[in] tryCount 探测包重复发出的次数
*/
int RSocket::setKeepAlive(bool enabled, int idleSec, int intervalSec, int tryCount)
{
    int t_iKeepAlive = 0;
    if(enabled)
        t_iKeepAlive = 1;

    int result = setSockopt(SO_KEEPALIVE,(char *)&t_iKeepAlive,sizeof(t_iKeepAlive));

#ifdef Q_OS_WIN
    if(result == 0){
        tcp_keepalive alivein;
        tcp_keepalive aliveout;
        alivein.onoff = TRUE;
        alivein.keepalivetime = idleSec * 1;
        alivein.keepaliveinterval = intervalSec * 1;

        unsigned long bytesReturn = 0;
        result = WSAIoctl(sockFd,SIO_KEEPALIVE_VALS,&alivein,sizeof(alivein),&aliveout,sizeof(aliveout),&bytesReturn,NULL,NULL);
    }
#elif deinfd(Q_OS_LINUX)
    setsockopt(sockFd,SOL_TCP,TCP_KEEPIDLE,(char *)&idle,sizeof(idle));
    setsockopt(sockFd,SOL_TCP,TCP_KEEPINVAL,(char *)&interval,sizeof(interval));
    setsockopt(sockFd,SOL_TCP,TCP_KEEPCNT,(char *)&tryCount,sizeof(tryCount));
#endif

    return result;
}

int RSocket::getLastError()
{
    return errorCode;
}

int RSocket::getErrorCode()
{
#ifdef Q_OS_WIN
    return WSAGetLastError();
#elif defined(Q_OS_LINUX)
    return -1;
#endif
}

}//namespace Network
