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

		enum NetConnectedFlag
		{
			DataConnected = 0x0001,		/*!< 数据链路已连接 */
			FileConnected = 0x0002,		/*!< 文件链路已连接 */
			AllConnected = DataConnected | FileConnected
		};
		Q_DECLARE_FLAGS(NetConnectedState, NetConnectedFlag)

		bool allNetConnected() const { return m_netState == AllConnected; }
		void startConnect(QString serverIp, ushort dataPort, ushort filePort);

		void setNetAutoConnect(bool isReconn,int maxReconnTimes);

	signals:
		void connectResult(Datastruct::ConnectionType type, bool connected, QString errorInfo = "");
		void reconnResult(Datastruct::ConnectionType type, int times);

	private slots:
		void respNetConnected(Datastruct::ConnectionType ctype, bool connected);
		void respReConnTimes(Datastruct::ConnectionType ctype, int times);

	private:
		ConnectorManage();

	private:
		static ConnectorManage * m_instance;

		NetConnectedState m_netState;
		bool m_isNetConnecting;		/*!< 网路是否处于连接中 */
	};

} //namespace Related 