/*!
 *  @brief     封装libuv的tcp服务器
 *  @details   1.支持并发连接；
 *             2.可对连接设置回调接口
 *  @author    wey
 *  @version   1.0
 *  @date      2020.02.25
 *  @warning
 *  @copyright NanJing RenGu.
 */
#ifndef UV_TCPSERVER_H
#define UV_TCPSERVER_H

#include <base\util\rringbuffer.h>
#include "uv_headfile.h"

namespace Network {

	class AcceptTcpClient;
	class Uv_TcpServer;

	typedef std::function<void(AcceptTcpClient *)> NewConnectionCallBack;
	typedef std::function<void(AcceptTcpClient *, const char* data, int dataLen)> MessageRecvCallBack;
	typedef std::function<void(AcceptTcpClient *)> MessageSendCallBack;
	typedef std::function<void(AcceptTcpClient *)> ConnClosedCallBack;
	typedef std::function<void()> ServerClosedCallBack;

	typedef function<void(AcceptTcpClient *, int)> AcceptClientWriteCallBack;

	class NETWORKSHARED_EXPORT Uv_TcpServer
	{
	public:
		Uv_TcpServer(uv_loop_t * eventLoop);
		~Uv_TcpServer();

		/*!
		 * @brief 开启服务器并在指定参属下运行
		 * @param[in] backlog 最大处理连接数
		 * @return true:启动成功，false:启动失败，可调用错误响应函数
		 */
		bool start(string localIp, int localPort, int backlog = 20);
		void stop();

		string errorInfo()const;

		/*!< socket属性设置 */
		bool setKeepAlive(bool enable, uint delaySeconds = 5);
		bool setNoDelay(bool enable);

		/*!< 用户回调接口 */
		void setNewConnectionCallBack(NewConnectionCallBack back) { m_newConnCallback = back; }
		void setMessageRecvCallback(MessageRecvCallBack back) { m_messRecvCallback = back; }
		void setMessageSendCallback(MessageSendCallBack back) { m_messSendCallback = back; }
		void setConnClosedCallback(ConnClosedCallBack back) { m_closedCallback = back; }

		void setServerClosedCallback(ServerClosedCallBack back) { m_serverClosedCallback = back; }

	private:
		bool init();
		bool bind();
		bool listen(int backLog = 20);
		void closing();

		int allocateClientId();

		static void acceptCB(uv_stream_t* server, int status);
		static void allocBufferForRecvCB(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf);
		static void recvDataCB(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf);
		static void asyncCloseCB(uv_async_t * handle);
		static void serverCloseCB(uv_handle_t* handle);
		static void freeAcceptFailedClient(uv_handle_t* handle);

		static void removeClient(AcceptTcpClient * client);

	private:
		string m_localIp;		/*!< 绑定本地ip */
		int m_localPort;		/*!< 绑定本地端口 */
		bool m_bClosed;
		bool m_bForceQuit;      /*!< 主动退出 */
		uv_async_t m_handleClose;	/*!< 线程间通信消息体 */

		uv_loop_t * m_eventLoop;
		uv_tcp_t m_tcpServer;	

		uv_mutex_t m_clientMutex;							/*!< 客户端句柄集合锁 */
		list<AcceptTcpClient *> m_clientList;				/*!< 客户端连接句柄集合 */

		NewConnectionCallBack m_newConnCallback;
		MessageRecvCallBack m_messRecvCallback;
		MessageSendCallBack m_messSendCallback;
		ConnClosedCallBack m_closedCallback;                /*!< 客户端连接关闭回调 */

		ServerClosedCallBack m_serverClosedCallback;        /*!< 服务器端关闭回调 */

		string m_errorMsg;
	};

	/*!
	 * @brief 服务器单个客户端连接请求对象
	 * @details 用于表示一个客户端与服务器之间的连接管道
	 */
	class NETWORKSHARED_EXPORT AcceptTcpClient
	{
	public:
		AcceptTcpClient(uv_loop_t * eventLoop, ClientConnHandle * handle);
		~AcceptTcpClient();

		void close();
		bool isClosed()const;

		int id() const;

		int send(const char * data, int len);

		void setWriteCallBack(AcceptClientWriteCallBack back) { m_writeCallback = back; }

	private:
		void setConnClosedCallBack(ConnClosedCallBack back) { m_closeCallback = back; }

		static void clientClose(uv_handle_t * handle);
		static void asyncCB(uv_async_t* handle);

		void sendData(uv_write_t * req);
		static void sendDataCB(uv_write_t *req, int status);

	private:
		uv_loop_t * m_eventLoop;
		ClientConnHandle * m_connHandle;
		int m_clientId;
		bool m_bForceQuit;      /*!< 主动退出 */
		bool m_bIsClosed;

		uv_async_t m_async;

		uv_mutex_t m_writeBuffMutex;
		list<WriteSegment*> m_sendList;         /*!< 待发送数据缓冲区队列 */
		Base::RFixedRingBuffer m_fixedRingBuffer;     /*!< 发送缓冲区 */

		ConnClosedCallBack m_closeCallback;
		AcceptClientWriteCallBack m_writeCallback;

		friend class Uv_TcpServer;
	};

} //namespace Network 

#endif // UV_TCPSERVER_H
