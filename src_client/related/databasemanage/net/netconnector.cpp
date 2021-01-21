#include "netconnector.h"

#include <QDebug>

#include "jsonwrapper.h"

namespace Related {

	NetConnector * NetConnector::m_instance = nullptr;

	NetConnector::NetConnector(QObject *parent)
		: QObject(parent)
	{
		initNetwork();
	}

	void NetConnector::initNetwork()
	{
		m_eventLoop = new Network::Uv_EventLoop();
		m_dataTcpClient = new Network::Uv_TcpClient(m_eventLoop->eventLoop());
		m_dataTcpClient->setConnectedCallBack(std::bind(&NetConnector::connectCallBack, this, std::placeholders::_1));
		m_dataTcpClient->setCloseCallBack(std::bind(&NetConnector::closeCallBack, this, std::placeholders::_1));

		m_eventLoop->startLoop();
	}

	void NetConnector::connectCallBack(Network::Uv_TcpClient * client)
	{
		if (client == nullptr) {
			emit netConnected(false);
			return;
		}
		emit netConnected(true);
	}

	void NetConnector::closeCallBack(Network::Uv_TcpClient * client)
	{
		emit netConnected(false);
	}

	NetConnector * NetConnector::instance()
	{
		if (m_instance == nullptr) {
			m_instance = new NetConnector();
		}

		return m_instance;
	}

	NetConnector::~NetConnector()
	{
	}

	void NetConnector::connectTo(QString remoteIp, ushort remotePort)
	{
		m_dataTcpClient->connect(remoteIp.toStdString(), remotePort);
	}

	bool NetConnector::isConnected()
	{
		return m_dataTcpClient->connected();
	}

	void NetConnector::write(PacketType type, const UserLoginRequest & request)
	{
		QByteArray array = makePacket(type, JsonWrapper::instance()->wrap(type, request));

		m_dataTcpClient->send(array.data(), array.length());
	}

	QByteArray NetConnector::makePacket(PacketType type, QByteArray & body)
	{
		static int headLen = sizeof(PacketHead);
		static int tailLen = sizeof(PacketTail);

		PacketHead phead;
		phead.m_packetType = type;
		phead.m_dataLen = headLen + body.length() + tailLen;

		PacketTail ptail;

		QByteArray data;
		data.append((char *)&phead, headLen);
		data.append(body);
		data.append((char *)&ptail, tailLen);

		return data;
	}

} //namespace Related 