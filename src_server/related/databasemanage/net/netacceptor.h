/*!
 * @brief     网络连接接收器
 * @details   用于创建tcp服务器，接收客户端连接
 * @author    wey
 * @version   1.0
 * @date      2021.01.22 14:45:06
 * @warning
 * @copyright NanJing RenGu.
 * @note
 */
 #pragma once

#include <QObject>

#include <network/libuv/uv_eventloop.h>
#include <network/libuv/uv_tcpserver.h>

#include "../datastruct.h"
#include "../net/protocol.h"

namespace Related {

	class NetAcceptor : public QObject
	{
		Q_OBJECT

	public:
		static NetAcceptor * instance();
		~NetAcceptor();

		void start(QString localIp,ushort localPort);

		void processResponseUnit(ResponseUnit * unit);

	private:
		NetAcceptor(QObject *parent = nullptr);

		void newTcpConnectionCallback(Network::AcceptTcpClient * remoteClient);
		void newMessageCallback(Network::AcceptTcpClient * remoteClient, const char* data, int dataLen);

		bool searchNextPackHead(Base::RFixedRingBuffer & ringBuffer);

	private:
		static NetAcceptor * m_instance;

		QMap<int, RemoteClientInfo * > m_clients;		/*!< key:连接编号，value:客户端连接句柄 */

		Network::Uv_EventLoop * m_dataEventLoop;		/*!< 数据模块事件循环 */
		Network::Uv_TcpServer * m_dataTcpServer;		/*!< 数据模块Tcp服务器 */

	};

} //namespace Related 