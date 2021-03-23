/*!
 * @brief     �����������ӹ���
 * @details   1.���𴴽��ͻ������ӣ����ӷ�������
			  2.������ͨ���ݰ�װ������
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
#include <QFile>
#include <QFileInfo>

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
		 * @brief ���ö��������������������
		 * @param isReconn true:������������,false:����������
		 * @param maxReconnTimes �������������ֻ��isReconnΪtrueʱ��Ч
		 * @return 
		 */
		void setNetAutoConnect(bool isReconn, int maxReconnTimes);

		/*!
		 * @brief ������ָ����ip�Ͷ˿ںŷ�������
		 * @attention ���ô˷�������Ҫ�ȴ�����netConnected�źţ����ж϶�Ӧ�Ĳ������ſ���ȷ�������Ƿ����ӳɹ�
		 * @param remoteIp Զ�̷�����IP
		 * @param remotePort Զ�̷������˿ں�
		 * @return true:���óɹ������������������ӳɹ���false:����ʧ��
		 */
		bool connectTo(QString remoteIp, ushort remotePort);

		/*!
		 * @brief ������������������Ƿ���
		 * @return true:�ѽ����������ӣ�false:δ������������
		 */
		bool isConnected();

	signals:
		void netConnected(Datastruct::ConnectionType type, bool isConnected);
		void reconnTimes(Datastruct::ConnectionType type,int times);
		void writeDataResult(Datastruct::ConnectionType type);

	protected:
		void initNetwork();

		virtual void processNetData(QByteArray & data) = 0;

		void connectCallBack(Network::Uv_TcpClient * client);
		void reconnCallBack(int tryTimes);
		void closeCallBack(Network::Uv_TcpClient * client);
		void recvDataCallBack(Network::Uv_TcpClient * remoteClient, const char * data, int dataLen);
		void writeDataCallBack(Network::Uv_TcpClient * client, int dataLen);
		bool searchNextPackHead();

		void sendData(const QByteArray & data);

	protected:
		Datastruct::ConnectionType m_connType;		/*!< ������������ */
		Network::Uv_EventLoop * m_eventLoop;		/*!< �¼�ѭ���߳� */
		Network::Uv_TcpClient * m_dataTcpClient;	/*!< ��ͨ�������� */

		Base::RFixedRingBuffer m_dataRecvRingBuffer;	/*!< ���ݽ��ջ��λ����� */

		int m_index;
	};

} //namespace Related 