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
		NetConnector(Datastruct::ConnectionType type,QObject *parent = nullptr);
		~NetConnector();

		/*! 
		 * @brief 设置断网重连和最大重连次数
		 * @param isReconn true:开启断网重连,false:不开启重连
		 * @param maxReconnTimes 最大重连次数，只在isReconn为true时有效
		 * @return 
		 */
		void setNetAutoConnect(bool isReconn, int maxReconnTimes);

		/*!
		 * @brief 主动向指定的ip和端口号发起连接
		 * @attention 调用此方法后，需要等待接收netConnected信号，并判断对应的参数，才可以确定网络是否连接成功
		 * @param remoteIp 远程服务器IP
		 * @param remotePort 远程服务器端口号
		 * @return true:调用成功，不不代表网络连接成功；false:调用失败
		 */
		bool connectTo(QString remoteIp, ushort remotePort);

		/*!
		 * @brief 与服务器的网络连接是否建立
		 * @return true:已建立网络连接；false:未建立网络连接
		 */
		bool isConnected();

	signals:
		void netConnected(Datastruct::ConnectionType type,bool isConnected);
		void reconnTimes(Datastruct::ConnectionType type,int times);

	protected:
		void initNetwork();

		virtual void processNetData(QByteArray & data) = 0;

		void connectCallBack(Network::Uv_TcpClient * client);
		void reconnCallBack(int tryTimes);
		void closeCallBack(Network::Uv_TcpClient * client);
		void recvDataCallBack(Network::Uv_TcpClient * remoteClient, const char * data, int dataLen);

		bool searchNextPackHead();

		void sendData(const QByteArray & data);

	protected:
		Datastruct::ConnectionType m_connType;		/*!< 网络连接类型 */
		Network::Uv_EventLoop * m_eventLoop;		/*!< 事件循环线程 */
		Network::Uv_TcpClient * m_dataTcpClient;	/*!< 普通数据连接 */

		Base::RFixedRingBuffer m_dataRecvRingBuffer;	/*!< 数据接收环形缓冲区 */
	};

} //namespace Related 