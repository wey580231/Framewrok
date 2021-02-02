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

	typedef function<void(int)> ReConnectCallBack;
	typedef function<void(Uv_TcpClient *)> ConnectedCallBack;
	typedef function<void(Uv_TcpClient *, const char *, int)>  RecvCallBack;
	typedef function<void(Uv_TcpClient *, int)> WriteCallBack;
	typedef function<void(Uv_TcpClient *)> CloseCallBack;

	class NETWORKSHARED_EXPORT Uv_TcpClient
	{
	public:
		Uv_TcpClient(uv_loop_t * loop);
		~Uv_TcpClient();

		/*! 
		 * @brief 设置是否自动重连
		 * @param flag true 自动重连
		 */
		void setAutoReconnect(bool flag = true);
		bool isAutoReconnect() const;
		void setMaxReconnectTimes(int maxTimes);
		void stopReconnect();

		/*! 
		 * @brief 获取当前重连时间周期
		 * @return 重连时间周期，单位ms
		 */
		int  reconnectInterval() const;

		/*!
		 * @brief 主动发起连接远程服务器
		 * @attention 返回值为true只表示调用成功，不表示已经连接成功
		 * @param[in] 远程主机Ip
		 * @param[in] 远程主机端口
		 * @param[in] 绑定本地主机ip
		 * @param[in] 绑定主机中端口,0表示不绑定,
		 * @return true:调用成功;false:调用失败
		 */
		bool connect(string remoteAddr, int port, string bindLocalIp = "", int bindLocalPort = 0);

		/*!
		 * @brief 将数据拷贝至发送缓冲区，返回实际拷贝的字节数
		 * @attention 只是将数据拷贝至缓冲区，并未实际发送出去
		 * @param[in] data 待发送数据起始位置
		 * @param[in] len 待发送数据总长度
		 * @return 实际拷贝至缓冲区的字节数
		 */
		int send(const char * data, int len);
		void close();

		bool connected() const;
		bool isClosed() const;
		string errorInfo()const;

		/*!< 用户回调接口 */
		void setReconnCallback(ReConnectCallBack back) { m_reconnectCallback = back; }
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

		void sendData();
		void closing();

	private:
		/*!
		 * @brief 连接状态
		 */
		enum ConnectedState {
			R_CLOSED,			/*!< 连接关闭，不可收发 */
			R_CONNECTING,		/*!< 网路正在连接，包括手动调用connect未返回，以及定时器重连过程 */
			R_ESTABLISHED,		/*!< 连接已经建立，可进行数据传输 */
			R_CLOSING,			/*!< 网络关闭中 */
			R_ERROR				/*!< 发生错误，需要重新进行初始化.可通过错误函数获取具体信息 */
		};

		inline bool isReconnecting() {return m_connectedState == R_CONNECTING;}

	private:
		TcpClientHandle * m_handle;
		uv_async_t m_aysnc;
		uv_loop_t * m_eventLoop;

		uv_connect_t m_connReq;
		ConnectedState m_connectedState;	/*!< 连接状态 */

		//网络重连
		uv_timer_t m_reconnectTimer;	/*!< 重连定时器 */
		bool m_bAutoReconnect;      /*!< 是否自动重连，默认为false */
		int m_maxReconnectTimes;	/*!< 最大重连次数，超过此次数停止重连，若还未建立连接，则提示连接失败。-1表示限制，>0表示重连指定次数 */
		int m_reconnectTimes;		/*!< 重连次数，每次连接建立后设置为0，每次重连时累加1 */
		int m_repeatConnTime;       /*!< 重复连接周期,单位s */
		bool m_bIsReconnecting;     /*!< 是否处于正在重连过程中,避免定时器多次回调 */

		string m_errorMsg;			/*!< 错误信息，在m_connectedState为ERROR时有效 */

		string m_remoteIp;          /*!< 远程服务器Ip */
		int m_remotePort;

		string m_bindLocalIp;       /*!< 绑定本机ip和端口 */
		int m_bindLocalPort;

		bool m_bForceClosed;        /*!< 是否强制关闭 */
		bool m_bIsClosed;           /*!< 是否已经关闭 */
		bool m_bExit;               /*!< 是否已经退出程序 */

		uv_mutex_t m_writeBuffMutex;
		list<WriteSegment*> m_cachedSendList;	/*!< 待发送数据区，新数据都加入此队列，唤醒发送线程 */
		list<WriteSegment*> m_sendList;         /*!< 发送数据队列，每次发送前，将待发送缓冲区的内容转移至当前容器 */
		Base::RFixedRingBuffer m_fixedRingBuffer;

		ReConnectCallBack m_reconnectCallback;
		ConnectedCallBack m_connectedCallback;
		RecvCallBack m_recvCallback;
		WriteCallBack m_writeCallback;
		CloseCallBack m_closeCallback;
	};

} //namespace Network

#endif // UV_TCPCLIENT_H
