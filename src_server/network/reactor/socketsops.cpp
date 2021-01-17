#include "socketsops.h"

#include <QDebug>

#ifdef Q_OS_WIN
#pragma  comment(lib,"ws2_32.lib")
#endif

namespace Network{

namespace Sock {

int rrecv(int fd, char *buff, int length)
{
    return recv(fd,buff,length,0);
}

int rsend(int fd, const char *buff, const int length)
{
    int ret = ::send(fd,buff,length,0);
    return ret;
}

int rconnect(int fd, const char *remoteIp, const unsigned short remotePort)
{
    sockaddr_in remoteAddr;
    remoteAddr.sin_family = AF_INET;
    remoteAddr.sin_port = htons(remotePort);
    remoteAddr.sin_addr.s_addr = inet_addr(remoteIp);

    //TODO 20191116将此功能移动至connector下进行监控
    int t_result = ::connect(fd,(sockaddr*)&remoteAddr,sizeof(remoteAddr));
    if(t_result != 0)
    {
        fd_set set;
        FD_ZERO(&set);
        FD_SET(fd,&set);
        timeval tm;
        tm.tv_sec = 3;
        tm.tv_usec = 0;
        if(select(fd+1,0,&set,0,&tm) <= 0)
        {
            return -1;
        }
    }
    return 0;
}

int createNonblockingSock()
{
    int t_fd = socket(AF_INET,SOCK_STREAM,0);
    setBlock(t_fd,false);
    return t_fd;
}

bool setBlock(int fd, bool isBlocking)
{
#if defined(Q_OS_WIN)
    unsigned long ul = 0;
    if(!isBlocking)
        ul = 1;

    return ioctlsocket(fd,FIONBIO,&ul);
#else defined(Q_OS_LINUX)
    int flags = fcntl(fd,F_GETFL,0);
    if(flags<0)
        return false;

    if(isBlocking)
        flags = flags&~O_NONBLOCK;
    else
        flags = flags|O_NONBLOCK;

    if(fcntl(fd,F_SETFL,flags)!=0)
        return false;
#endif
}

int rbind(int fd, const char *localIp, ushort port)
{
    sockaddr_in s_in;
    s_in.sin_addr.s_addr = inet_addr(localIp);
    s_in.sin_family = AF_INET;
    s_in.sin_port = htons(port);

    return bind(fd,(sockaddr *)&s_in,sizeof(sockaddr));
}

int rlisten(int fd)
{
    return ::listen(fd,20);
}

int rclose(int fd)
{
    return closesocket(fd);
}

int setSendBuff(int sockFd,int bytes)
{
   return setsockopt(sockFd,SOL_SOCKET,SO_SNDBUF,(char *)&bytes,sizeof(bytes));
}

int setRecvBuff(int sockFd,int bytes)
{
    return setsockopt(sockFd,SOL_SOCKET,SO_RCVBUF,(char *)&bytes,sizeof(bytes));
}

void rshutdownWrite(int fd)
{
    if(shutdown(fd,SD_SEND) < 0){
        qDebug()<<"shut down error~";
    }
}

int createUdpSock()
{
    int t_fd = socket(AF_INET,SOCK_DGRAM,0);
    setBlock(t_fd,false);
    return t_fd;
}

int recvFrom(int fd, char *buff, int length)
{
    sockaddr remoteAddr;
    int len = sizeof(sockaddr);
    return recvfrom(fd,buff,length,0,&remoteAddr,&len);
}

int sendTo(int fd, const char *buff, int len, const char *ip, ushort port)
{
    sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = inet_addr(ip);
    sin.sin_port = htons(port);

    return sendto(fd,buff,len,0,(sockaddr *)&sin,sizeof(sockaddr));
}

/*!
* @brief 是否开启KeepAlive检测
* @details 为了检测链路是否正常，特别是网线突然断掉特殊情况下。由系统主动发送探测包，用来检测链路是否
* @param[in] enabled true时开启、false是关闭
* @param[in] idle 出发检测的时间间隔(此时间内没有数据来往),单位s
* @param[in] interval 探测包发出的时间间隔，单位s
* @param[in] tryCount 探测包重复发出的次数
*/
int setKeepAlive(int sockFd, bool enabled, int idleSec, int intervalSec, int tryCount)
{
    int t_iKeepAlive = 0;
    if(enabled)
        t_iKeepAlive = 1;

    int result = setsockopt(sockFd,SOL_SOCKET,SO_KEEPALIVE,(char *)&t_iKeepAlive,sizeof(t_iKeepAlive));

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

int setReuseAddr(int sockFd, bool flag)
{
    return setsockopt(sockFd,SOL_SOCKET,SO_REUSEADDR,(char *)&flag,sizeof(flag));
}

} //namespace Sock

} //namespace Network
