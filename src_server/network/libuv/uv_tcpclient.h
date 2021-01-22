/*!
 *  @brief     封装libuv的tcp客户端
 *  @details
 *             1.支持主动连接，断线重连等功能；
 *             2.支持用户设置回调接口，处理不同阶段问题；
 *             3.支持设置socket的keepalive、控制开关nagle算法
 *  @author    wey
 *  @version   1.0
 *  @date      2020.02.25
 *  @warning
 *  @copyright NanJing RenGu.
 */
#ifndef UV_TCPCLIENT_H
#define UV_TCPCLIENT_H

#include <base\util\rringbuffer.h>

#include "uv_headfile.h"

namespace Network {

	class Uv_TcpClient;

	typedef function<void(Uv_TcpClient *)> ConnectedCallBack;
	typedef function<void(Uv_TcpClient *, const char *, int)>  RecvCallBack;
	typedef function<void(Uv_TcpClient *, int)> WriteCallBack;
	typedef function<void(Uv_TcpClient *)> CloseCallBack;

	class NETWORKSHARED_EXPORT Uv_TcpClient
	{
	public:
		Uv_TcpClient(uv_loop_t * loop);
		~Uv_TcpClient();

		void setAutoReconnect(bool flag = true);
		bool isAutoReconnect() const;

		bool connect(string remoteAddr, int port, string bindLocalIp = "", int bindLocalPort = 0);
		int send(const char * data, int len);
		void close();

		bool connected() const;
		bool isClosed() const;
		string errorInfo()const;

		/*!< 用户回调接口 */
		void setConnectedCallBack(ConnectedCallBack back) { m_connectedCallback = back; }
		void setRecvCallback(RecvCallBack back) { m_recvCallback = back; }
		void setWriteCallBack(WriteCallBack back) { m_writeCallback = back; }
		void setCloseCallBack(CloseCallBack back) { m_closeCallback = back; }

		/*!< socket属性设置 */
		bool setKeepAlive(bool enable, uint delaySeconds = 5);
		bool setNoDelay(bool enable);

	private:
		bool init();
		bool bindLocalAddress();

		static void aysncCB(uv_async_t* handle);
		static void connectCB(uv_connect_t* handle, int status);
		static void reconnectTimerCB(uv_timer_t* handle);
		static void allocBufferForRecvCB(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf);
		static void recvDataCB(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf);
		static void sendDataCB(uv_write_t* req, int status);
		static void remoteClientCloseCB(uv_handle_t * handle);
		static void localClientCloseCB(uv_handle_t * handle);

		void sendData(uv_write_t * req);
		void stopReconnect();
		void closing();

	private:
		/*!
		 * @brief 连接状态
		 */
		enum ConnectedState {
			R_CLOSED,
			R_CONNECTING,
			R_ESTABLISHED,
			R_CLOSING,
			R_ERROR
		};

	private:
		TcpClientHandle * m_handle;
		uv_async_t m_aysnc;
		uv_loop_t * m_eventLoop;

		uv_connect_t m_connReq;
		ConnectedState m_connectedState;

		uv_timer_t m_reconnectTimer;    /*!< 重连定时器 */
		bool m_bAutoReconnect;      /*!< 是否自动重连，默认为false */
		int m_repeatConnTime;       /*!< 重复连接周期,单位s */
		bool m_bIsReconnecting;     /*!< 是否处于正在重连过程中,避免定时器多次回调 */

		string m_errorMsg;

		string m_remoteIp;          /*!< 远程服务器Ip */
		int m_remotePort;

		string m_bindLocalIp;       /*!< 绑定本机ip和端口 */
		int m_bindLocalPort;

		bool m_bForceClosed;        /*!< 是否强制关闭 */
		bool m_bIsClosed;           /*!< 是否已经关闭 */
		bool m_bExit;               /*!< 是否已经退出程序 */

		uv_mutex_t m_writeBuffMutex;
		list<WriteSegment*> m_sendList;         /*!< 待发送数据缓冲区队列 */
		Base::RFixedRingBuffer m_fixedRingBuffer;

		ConnectedCallBack m_connectedCallback;
		RecvCallBack m_recvCallback;
		WriteCallBack m_writeCallback;
		CloseCallBack m_closeCallback;
	};

} //namespace Network

#endif // UV_TCPCLIENT_H
