#pragma once

#include <QObject>
#include <QSharedPointer>
#include <QMutex>

#include <commondefines/protocol.h>
#include <network/libuv/uv_eventloop.h>
#include <network/libuv/uv_tcpserver.h>

#include "filesession.h"
#include "../datastruct.h"

namespace Related {

	typedef QSharedPointer<FileSession> FileSessionPtr;

	class FileServer : public QObject
	{
		Q_OBJECT

	public:
		static FileServer * instance();
		~FileServer();

		void start(QString localIp, ushort localPort);

		void processResponseUnit(ResponseUnit * unit);

	private:
		FileServer(QObject *parent = nullptr);

		void newTcpConnectionCallback(Network::AcceptTcpClient * remoteClient);
		void newMessageCallback(Network::AcceptTcpClient * remoteClient, const char* data, int dataLen);

	private:
		static FileServer * m_instance;

		QMap<int, FileSessionPtr> m_clients;			/*!< key:连接编号，value:客户端连接句柄 */

		Network::Uv_EventLoop * m_dataEventLoop;		/*!< 数据模块事件循环 */
		Network::Uv_TcpServer * m_dataTcpServer;		/*!< 数据模块Tcp服务器 */
		QMutex m_clientMutex;
	};

} //namespace Related 