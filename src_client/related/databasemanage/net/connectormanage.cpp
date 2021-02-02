#include "connectormanage.h"

#include "datanetconnector.h"
#include "filenetconnector.h"

namespace Related {

	ConnectorManage * ConnectorManage::m_instance = nullptr;

	ConnectorManage::ConnectorManage()
		: QObject()
	{

		connect(DataNetConnector::instance(), SIGNAL(netConnected(Datastruct::ConnectionType, bool)), this, SLOT(respNetConnected(Datastruct::ConnectionType, bool)));
		connect(FileNetConnector::instance(), SIGNAL(netConnected(Datastruct::ConnectionType, bool)), this, SLOT(respNetConnected(Datastruct::ConnectionType, bool)));

		//NOTE 默认提供自动重连机制，用户也可以手动连接
		DataNetConnector::instance()->setAutoReconnect(true);
		FileNetConnector::instance()->setAutoReconnect(true);
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
		if (!DataNetConnector::instance()->isConnected()) {
			if (DataNetConnector::instance()->connectTo(serverIp, dataPort)) {

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

			}
			else {
				emit connectResult(Datastruct::Data_Connection, false, QStringLiteral("连接文件服务器失败!"));
			}
		}
		else {
			emit(Datastruct::File_Connection, true);
		}
	}

	void ConnectorManage::respNetConnected(Datastruct::ConnectionType ctype, bool connected)
	{
		if (connected) {
			emit connectResult(ctype, true);
		}
		else {
			if (ctype == Datastruct::Data_Connection) {
				emit connectResult(Datastruct::Data_Connection, false, QStringLiteral("连接数据服务器失败!"));
			}
			else {
				emit connectResult(Datastruct::Data_Connection, false, QStringLiteral("连接文件服务器失败!"));
			}
		}
	}

} //namespace Related 