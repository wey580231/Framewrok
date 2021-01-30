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
#include <QSharedPointer>
#include <QMutex>

#include <commondefines/protocol.h>
#include <network/libuv/uv_eventloop.h>
#include <network/libuv/uv_tcpserver.h>

#include "clientsession.h"
#include "../datastruct.h"

namespace Related {

	typedef QSharedPointer<ClientSession> ClientSessionPtr;

	class MsgServer : public QObject
	{
		Q_OBJECT

	public:
		static MsgServer * instance();
		~MsgServer();

		void start(QString localIp,ushort localPort);

		void processResponseUnit(ResponseUnit * unit);

	private:
		MsgServer(QObject *parent = nullptr);

		void newTcpConnectionCallback(Network::AcceptTcpClient * remoteClient);
		void newMessageCallback(Network::AcceptTcpClient * remoteClient, const char* data, int dataLen);

	private:
		static MsgServer * m_instance;

		QMap<int, ClientSessionPtr> m_clients;		/*!< key:连接编号，value:客户端连接句柄 */

		Network::Uv_EventLoop * m_dataEventLoop;		/*!< 数据模块事件循环 */
		Network::Uv_TcpServer * m_dataTcpServer;		/*!< 数据模块Tcp服务器 */
		QMutex m_clientMutex;
	};

} //namespace Related 