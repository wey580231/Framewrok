/*!
 * @brief     �ļ��������ӽ�����
 * @details 
 * @author    wey
 * @version   1.0
 * @date      2021.03.10 08:19:04
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

		QMap<int, FileSessionPtr> m_clients;			/*!< key:���ӱ�ţ�value:�ͻ������Ӿ�� */

		Network::Uv_EventLoop * m_dataEventLoop;		/*!< ����ģ���¼�ѭ�� */
		Network::Uv_TcpServer * m_dataTcpServer;		/*!< ����ģ��Tcp������ */
		QMutex m_clientMutex;
	};

} //namespace Related 