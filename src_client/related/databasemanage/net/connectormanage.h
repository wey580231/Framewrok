/*!
 * @brief     网络连接管理，负责维护数据、文件两个连接的装填，若出现断连，则自动重连   
 * @author    wey
 * @version   1.0
 * @date      2021.02.02 08:49:51
 * @warning
 * @copyright NanJing RenGu.
 * @note
 */
 #pragma once

#include <QObject>

#include <commondefines/structdefines.h>

namespace Related {

	class ConnectorManage : public QObject
	{
		Q_OBJECT

	public:
		static ConnectorManage * instance();
		~ConnectorManage();

		void startConnect(QString serverIp,ushort dataPort,ushort filePort);

	signals:
		void connectResult(Datastruct::ConnectionType type,bool connected,QString errorInfo = "");

	private slots:
		void respNetConnected(Datastruct::ConnectionType ctype, bool connected);

	private:
		ConnectorManage();

	private:
		static ConnectorManage * m_instance;
	};

} //namespace Related 