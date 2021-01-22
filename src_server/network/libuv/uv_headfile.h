/*!
 *  @brief     各个平台的头文件
 *  @details
 *  @author    wey
 *  @version   1.0
 *  @date      2020.02.13
 *  @warning
 *  @copyright NanJing RenGu.
 */
#ifndef SYSTEMHEADFILE_H
#define SYSTEMHEADFILE_H

#ifdef WIN32
#define OS_WIN 1
#else
#define OS_LINUX 1
#endif

#define Check_Return(condition,returnVal) {if(condition) return returnVal;}
#define Check_Assert(condition,format,...) {if(condition){RLOG_ERROR(format,__VA_ARGS__);throw 0;}}
#define Check_Assert_S(condition,errorMsg) {if(condition){RLOG_ERROR_S(errorMsg);throw 0;}}

#define R_UNUSED(para) (void)para

#define DKB 1024.0
#define KB 1024
#define MB (1024*1024)
#define GB (1024*1024*1024)

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;
typedef long long longlong;
typedef unsigned long long ulonglong;

#include <cstdio>
#include <cstring>
#include <string>
#include <ctime>
#include <iostream>
#include <vector>
#include <list>
#include <functional>
#include <thread>
#include <array>
#include <map>

using namespace std;

#include <uv.h>

#include "../network_global.h"

#define BUFFER_SIZE (1024*64)
#define MAX_SEND_LIST_SIZE 20

/*!
 * @brief uv_tcpclient客户端连接句柄
 */
struct TcpClientHandle
{
	uv_tcp_t client;
	uv_buf_t readBuf;           /*!< 接收暂存缓冲区 */

	void * uv_tcpClient;        /*!< 客户端连接 */
};

/*!
 * @brief 单块数据发送段
 */
struct WriteSegment {
	uv_write_t writeReq;
	uv_buf_t dataBuff;
	int dataLen;
};

/*!
 * @brief 服务器保存远程连接句柄
 */
struct ClientConnHandle {
	uv_tcp_t tcpConn;
	uv_buf_t readBuff;
	int clientId;           /*!< 客户端标志，大于使用 */
	void * server;          /*!< 所属服务器 */
	void * acceptClient;    /*!< 关联的接收client */
};

extern WriteSegment* allocWriteSegment(int fullDataSize);
extern void freeWriteSegment(WriteSegment* param);

/*! 
 * @brief 根据错误码获取libuv的错误描述信息
 * @param erroCode 错误码
 * @return 错误描述信息
 */
extern string getLastUvError(int erroCode);

/*! 
 * @brief 线程水面指定的时间
 * @param millsecond 睡眠时间ms
 */
extern void msleep(int millsecond);

#endif // SYSTEMHEADFILE_H
