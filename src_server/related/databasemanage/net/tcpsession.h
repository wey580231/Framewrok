/*!
 * @brief     TCP客户端连接
 * @details   封装了网络数据的解包、发送过程，客户端数据连接和文件连接可继承此类，完成对请求的处理
 * @author    wey
 * @version   1.0
 * @date      2021.01.30 14:42:08
 * @warning
 * @copyright NanJing RenGu.
 * @note
 */
 #pragma once

#include <QObject>

#include <base/util/rringbuffer.h>
#include <commondefines/protocol.h>
#include <network/libuv/uv_tcpserver.h>

namespace Related {

	class TcpSession
	{
	public:
		TcpSession(Network::AcceptTcpClient * client);
		~TcpSession();

		void sendData(const char * data, int dataLength);

		void recvData(const char * data, int dataLength);

	protected:
		/*! 
		 * @brief 处理去除包头包尾后的请求数据体
		 * @details 子类根据需要可对请求体进行不同的处理
		 * @param requestData 已经被解包的数据请求体
		 */
		virtual void processData(QByteArray & requestData) = 0;

	private:
		bool searchNextPackHead();

	protected:
		Network::AcceptTcpClient * m_tcpClient;		/*!< 客户端连接 */
		Base::RFixedRingBuffer m_recvRingBuffer;	/*!< 客户端对应的接收环形缓冲区 */
	};

} //namespace Related 