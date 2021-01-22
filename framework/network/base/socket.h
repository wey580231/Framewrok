/*!
 *  @brief     windows底层socket
 *  @details   封装了操作系统底层的socket，支持windows和linux平台的运行
 *  @file      socket.h
 *  @author    wey
 *  @version   1.0
 *  @date      2018.01.09
 *  @warning
 *  @copyright NanJing RenGu.
 */

#ifndef RSOCKET_H_2018_01_09
#define RSOCKET_H_2018_01_09

#include <cstring>
#include "../network_global.h"

namespace Network {

class NETWORKSHARED_EXPORT  RSocket
{
public:
    RSocket();
    RSocket(const RSocket & rsock);

    enum SocketType{
        R_NONE = 0,
        R_TCP = 1,          /*!< 创建tcp连接 */
        R_UDP = 2,          /*!< 创建udp连接 */
        R_RAW = 3           /*!< 创建原始套接字连接 */
    };

    bool createSocket(SocketType socktype);
    bool closeSocket();

    bool bind(const char * ip,unsigned short port);
    bool listen(int backlog = 20);
    RSocket accept();

    unsigned short port(){return socketPort;}
    std::string ip(){return std::string(socketIp);}

    /*!< TCP */
    int recv(char * buff,int length);
    int send(const char * buff,const int length);

    bool connect(const char * remoteIp,const unsigned short remotePort,int timeouts = 3);

    bool setBlock(bool flag);
    bool isBock(){return blockAble;}

    /*!< UDP */
    int recvFrom(char * buff, int length, std::string &remoteIp, ushort &remotePort);
    int sendTo(const char * buff,const int length,const char * dest,const int port);
    bool joinGroup(const char* groupIp);

    /*!< 设置套接字属性 */
    int setSockopt(int optname,const char * optval,int optlen);
    int setSendBuff(int bytes);
    int setRecvBuff(int bytes);

    int setRecvTimeOut(int millsecond = 3000);
    int setSendTimeOut(int millsecond = 3000);

    /*!< 读取套接字属性 */
    int getSockopt(int optname, char *optval, int optlen);

    int getSendBuff();
    int getRecvBuff();

    int getSendTimeout();
    int getRecvTimeOut();

    int setKeepAlive(bool enabled, int idleSec, int intervalSec, int tryCount);

    bool isValid(){return socketValid;}

    int getLastError();

    int getSocket()const {return sockFd;}
    SocketType getSocketType()const{return this->socktype;}

private:
    int getErrorCode();

private:
    char socketIp[20];          /*!< 客户端IP地址 */
    unsigned short socketPort;

    bool socketValid;
    bool blockAble;

    int sockFd;
    int errorCode;

    SocketType socktype;

    sockaddr_in localAddr;
};

}   //namespace Network

#endif // TCPSOCKET_H
