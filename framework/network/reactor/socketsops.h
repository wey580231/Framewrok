#ifndef SOCKETSOPS_H
#define SOCKETSOPS_H

#include "callbacks.h"

namespace Network{

namespace Sock {

/*!< TCP */
bool setBlock(int fd,bool isBlocking);

int createNonblockingSock();
int rrecv(int fd,char * buff,int length);
int rsend(int fd,const char * buff,const int length);

int rbind(int fd,const char * localIp,ushort port);
int rlisten(int fd);
int rconnect(int fd, const char * remoteIp, const unsigned short remotePort);
int rclose(int fd);
void rshutdownWrite(int fd);

int setSendBuff(int sockFd, int bytes);
int setRecvBuff(int sockFd, int bytes);

int setKeepAlive(int sockFd, bool enabled, int idleSec, int intervalSec, int tryCount);
int setReuseAddr(int sockFd,bool flag);

/*!< UDP */
int createUdpSock();
int recvFrom(int fd,char * buff,int length);
int sendTo(int fd,const char * buff, int len, const char * ip, ushort port);

} //namespace Sock

}  //namespace Network

#endif // SOCKETSOPS_H
