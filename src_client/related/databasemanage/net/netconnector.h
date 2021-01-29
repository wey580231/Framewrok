/*!
 * @brief     网络数据连接管理
 * @details   1.负责创建客户端连接，连接服务器；
			  2.负责普通数据包装、发送
 * @author    wey
 * @version   1.0
 * @date      2021.01.20 16:06:15
 * @warning
 * @copyright NanJing RenGu.
 * @note
 */
 #pragma once

#include <QObject>
#include <QDateTime>

#include <base\util\rringbuffer.h>
#include <commondefines/protocol.h>
#include <network\libuv\uv_eventloop.h>
#include <network\libuv\uv_tcpclient.h>

namespace Related {

	class NetConnector : public QObject
	{
		Q_OBJECT
	public:
		static NetConnector * instance();
		~NetConnector();

		void setAutoReconnect(bool isAutoReconnect);

		/*! 
		 * @brief 主动向指定的ip和端口号发起连接
		 * @attention 调用此方法后，需要等待接收netConnected信号，并判断对应的参数，才可以确定网络是否连接成功
		 * @param remoteIp 远程服务器IP
		 * @param remotePort 远程服务器端口号
		 * @return true:调用成功，不不代表网络连接成功；false:调用失败
		 */
		bool connectTo(QString remoteIp,ushort remotePort);

		/*! 
		 * @brief 与服务器的网络连接是否建立
		 * @return true:已建立网络连接；false:未建立网络连接
		 */
		bool isConnected();

		/*!
		 * @brief   用户有关
		 */
		void write(const Datastruct::UserLoginRequest & request);
		void write(const Datastruct::UserRegistRequest & request);
		void write(const Datastruct::LoadAllUserRequest & request);
		void write(const Datastruct::OperateUserRequest & request);

		/*!
		 * @brief 任务操作有关
		 */
		void write(const Datastruct::TaskCreateRequest & request);
		void write(const Datastruct::LoadAllTaskRequest & request);
		void write(const Datastruct::TaskDeleteRequest & request);
		void write(const Datastruct::TaskSimpleRequest & request);
		
		/*!
		 * @brief  值班日志有关
		 */
		void write(const Datastruct::DutyRecordCreateRequest & request);
		void write(const Datastruct::LoadAllDutyRecordRequest & request);
		void write(const Datastruct::DutyRecordDeleteRequest & request);
		void write(const Datastruct::DutyRecordModifyRequest & request);
		
		/*!
		 * @brief   试验记录有关
		 */
		void write(const Datastruct::ExperimentRecordCreateRequest & request);
		void write(const Datastruct::LoadAllExperimentRecordsRequest & request);
		void write(const Datastruct::ExperimentRecordDeleteRequest & request);
		void write(const Datastruct::ExperimentRecordModifyRequest & request);
		
		/*!
		 * @brief   侦测平台有关
		 */
		void write(const Datastruct::DetectPlatformCreateRequest & request);
		void write(const Datastruct::LoadAllDetectPlatformsRequest & request);
		void write(const Datastruct::DetectPlatformDeleteRequest & request);
		void write(const Datastruct::DetectPlatformModifyRequest & request);

		/*!
		 * @brief   侦测平台亚型有关
		 */
		void write(const Datastruct::DetectPlatformSubtypeCreateRequest & request);
		void write(const Datastruct::LoadAllDetectPlatformSubtypesRequest & request);
		void write(const Datastruct::DetectPlatformSubtypeDeleteRequest & request);
		void write(const Datastruct::DetectPlatformSubtypeModifyRequest & request);

	signals:
		void netConnected(bool isConnected);
		void netRecvData(QByteArray array);

	private slots:
		void respRectNetData(QByteArray array);

	private:
		NetConnector(QObject *parent = nullptr);
		void initNetwork();

		void connectCallBack(Network::Uv_TcpClient * client);
		void closeCallBack(Network::Uv_TcpClient * client);
		void recvDataCallBack(Network::Uv_TcpClient * remoteClient, const char * data, int dataLen);

		bool searchNextPackHead();

		QByteArray makePacket(Datastruct::PacketType type,QByteArray & body);
		void sendData(const QByteArray & data);

	private:
		static NetConnector * m_instance;

		Network::Uv_EventLoop * m_eventLoop;		/*!< 事件循环线程 */
		Network::Uv_TcpClient * m_dataTcpClient;	/*!< 普通数据连接 */

		Base::RFixedRingBuffer m_dataRecvRingBuffer;	/*!< 数据接收环形缓冲区 */
	};

} //namespace Related 