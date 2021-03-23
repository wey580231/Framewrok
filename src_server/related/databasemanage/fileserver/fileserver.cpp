#include "fileserver.h"

#include <QDebug>

namespace Related {

	FileServer * FileServer::m_instance = nullptr;

	FileServer::FileServer(QObject *parent)
		: QObject(parent)
	{
	}

	FileServer * FileServer::instance()
	{
		if (m_instance == nullptr)
			m_instance = new FileServer();

		return m_instance;
	}

	FileServer::~FileServer()
	{
	}

	void FileServer::start(QString localIp, ushort localPort)
	{
		m_dataEventLoop = new Network::Uv_EventLoop();

		m_dataTcpServer = new Network::Uv_TcpServer(m_dataEventLoop->eventLoop());
		m_dataTcpServer->setNewConnectionCallBack(std::bind(&FileServer::newTcpConnectionCallback, this, std::placeholders::_1));
		m_dataTcpServer->setMessageRecvCallback(std::bind(&FileServer::newMessageCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

		m_dataTcpServer->start(localIp.toStdString(), localPort);
		m_dataEventLoop->startLoop();
	}

	/*!
	 * @brief ����Ӧ���ͻؿͻ���
	 * @details �����ݿͻ��˽���Ϣ���ɵ����Ĵ����̴߳����������������Ϊ�漰���߳�֮���client�ķ��ʣ�
				������Ҫ����
	 * @param unit ��������Ӧ
	 */
	void FileServer::processResponseUnit(ResponseUnit * unit)
	{
		{
			QMutexLocker locker(&m_clientMutex);

			if (m_clients.contains(unit->m_clientId)) {
				FileSessionPtr client = m_clients.value(unit->m_clientId);
				client->sendData(unit->m_resposneData.data(), unit->m_resposneData.size());
			}
		}

		delete unit;
	}

	/*!
	 * @brief �����¿ͻ�����������
	 * @param remoteClient �ͻ�������
	 */
	void FileServer::newTcpConnectionCallback(Network::AcceptTcpClient * remoteClient)
	{
		FileSessionPtr ptr(new FileSession(remoteClient));

		{
			QMutexLocker locker(&m_clientMutex);
			m_clients.insert(remoteClient->id(), ptr);
		}
	}

	/*!
	 * @brief �ͻ��˽����������ݺ�ص�
	 * @param remoteClient �������ݵĿͻ���
	 * @param data ��������ʼ��ַ
	 * @param dataLen �������ݵĳ���
	 */
	void FileServer::newMessageCallback(Network::AcceptTcpClient * remoteClient, const char* data, int dataLen)
	{
		QMutexLocker locker(&m_clientMutex);
		if (!m_clients.contains(remoteClient->id())) {
			return;
		}

		FileSessionPtr clientInfo = m_clients.value(remoteClient->id());
		clientInfo->recvData(data, dataLen);
	}

} //namespace Related 