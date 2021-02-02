#include "connectormanage.h"

#include "datanetconnector.h"
#include "filenetconnector.h"

namespace Related {

	ConnectorManage * ConnectorManage::m_instance = nullptr;

	ConnectorManage::ConnectorManage()
		: QObject(), m_netState(0), m_isNetConnecting(false)
	{
		connect(DataNetConnector::instance(), SIGNAL(netConnected(Datastruct::ConnectionType, bool)), this, SLOT(respNetConnected(Datastruct::ConnectionType, bool)));
		connect(DataNetConnector::instance(), SIGNAL(reconnTimes(Datastruct::ConnectionType, int)), this, SLOT(respReConnTimes(Datastruct::ConnectionType, int)));

		connect(FileNetConnector::instance(), SIGNAL(netConnected(Datastruct::ConnectionType, bool)), this, SLOT(respNetConnected(Datastruct::ConnectionType, bool)));
		connect(FileNetConnector::instance(), SIGNAL(reconnTimes(Datastruct::ConnectionType, int)), this, SLOT(respReConnTimes(Datastruct::ConnectionType, int)));
	}

	ConnectorManage * ConnectorManage::instance()
	{
		if (m_instance == nullptr)
			m_instance = new ConnectorManage();

		return m_instance;
	}

	ConnectorManage::~ConnectorManage()
	{
	}

	void ConnectorManage::startConnect(QString serverIp, ushort dataPort, ushort filePort)
	{
		if (m_isNetConnecting)
			return;

		if (!DataNetConnector::instance()->isConnected()) {
			if (DataNetConnector::instance()->connectTo(serverIp, dataPort)) {
				m_isNetConnecting = true;
			}
			else {
				emit connectResult(Datastruct::Data_Connection,false, QStringLiteral("连接数据服务器失败!"));
			}
		}
		else {
			emit(Datastruct::Data_Connection,true);
		}

		if (!FileNetConnector::instance()->isConnected()) {
			if (FileNetConnector::instance()->connectTo(serverIp, filePort)) {
				m_isNetConnecting = true;
			}
			else {
				emit connectResult(Datastruct::Data_Connection, false, QStringLiteral("连接文件服务器失败!"));
			}
		}
		else {
			emit(Datastruct::File_Connection, true);
		}
	}

	void ConnectorManage::setNetAutoConnect(bool isReconn, int maxReconnTimes)
	{
		DataNetConnector::instance()->setNetAutoConnect(isReconn, maxReconnTimes);
		FileNetConnector::instance()->setNetAutoConnect(isReconn, maxReconnTimes);

		m_isNetConnecting = false;
	}

	void ConnectorManage::respNetConnected(Datastruct::ConnectionType ctype, bool connected)
	{
		m_isNetConnecting = false;

		if (connected) {
			if (ctype == Datastruct::Data_Connection)
				m_netState |= DataConnected;
			else if (ctype == Datastruct::File_Connection)
				m_netState |= FileConnected;

			emit connectResult(ctype, true);
		}
		else {
			if (ctype == Datastruct::Data_Connection) {
				m_netState &= ~DataConnected;
				emit connectResult(Datastruct::Data_Connection, false, QStringLiteral("连接数据服务器失败!"));
			}
			else {
				m_netState &= ~FileConnected;
				emit connectResult(Datastruct::Data_Connection, false, QStringLiteral("连接文件服务器失败!"));
			}
		}
	}

	void ConnectorManage::respReConnTimes(Datastruct::ConnectionType ctype, int times)
	{
		emit reconnResult(ctype, times);
	}

} //namespace Related 