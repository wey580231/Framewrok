#include "tcpsession.h"

#include <QDebug>

namespace Related {

	TcpSession::TcpSession(Network::AcceptTcpClient * client)
		: m_tcpClient(client)
	{
	}

	TcpSession::~TcpSession()
	{
	}

	void TcpSession::sendData(const char * data, int dataLength)
	{
		m_tcpClient->send(data, dataLength);
	}

	void TcpSession::recvData(const char * data, int dataLength)
	{
		m_recvRingBuffer.append(data, dataLength);

		Datastruct::PacketHead packHead;
		int packHeadLen = sizeof(Datastruct::PacketHead);

		if (m_recvRingBuffer.dataSize() > packHeadLen)
		{
			do
			{
				m_recvRingBuffer.preRead((char *)&packHead, packHeadLen);

				if (packHead.m_magicHead == PACK_HEAD)
				{
					//[1]至少存在多余一个完整数据包
					if (packHead.m_dataLen <= m_recvRingBuffer.dataSize())
					{
						int endCode = 0;
						//NOTE 20190711 接收到数据长度为0时，需重新寻找下一个数据头
						int t_offsetRead = packHead.m_dataLen - sizeof(Datastruct::PacketTail);
						if (t_offsetRead > 0) {
							m_recvRingBuffer.preRead(t_offsetRead, (char *)&endCode, sizeof(int));
						}
						else {
							if (searchNextPackHead())
								continue;
						}

						//NOTE 20190710 在没有找到尾标志时，应该从当前位置开始去搜索下一个起始标识，避免网络数据一直堆积
						if (endCode != PACK_TAIL) {
							qDebug() << "start search next frame:" << packHead.m_packetType << packHead.m_dataLen <<m_recvRingBuffer.dataSize();
							if (searchNextPackHead())
								continue;
							else
								return;
						}

						//NOTE 请求包已经被去除包头包尾，交由子类自行处理
						QByteArray dataArray;
						dataArray.resize(packHead.m_dataLen);
						m_recvRingBuffer.read(dataArray.data(), packHead.m_dataLen);

						processData(dataArray);

						if (m_recvRingBuffer.dataSize() <= 0)
							break;

						if (m_recvRingBuffer.dataSize() >= packHeadLen)
							continue;
					}
					else {
						break;
					}
				}
				else
				{
					if (searchNextPackHead())
						continue;
					else
						return;
				}
			} while (m_recvRingBuffer.dataSize() > 0);
		}
	}

	bool TcpSession::searchNextPackHead()
	{
		qint64 t_iHasSearchLen = 1;
		int t_iPackHeadLen = sizeof(Datastruct::PacketHead);
		Datastruct::PacketHead t_packHead;

		uint dd = PACK_HEAD;

		while (t_iHasSearchLen <= m_recvRingBuffer.dataSize() - t_iPackHeadLen)
		{
			m_recvRingBuffer.preRead(t_iHasSearchLen, (char *)&t_packHead, t_iPackHeadLen);
			if (t_packHead.m_magicHead == PACK_HEAD)
			{
				qDebug() << "++searchNextPackHead data:" << t_iHasSearchLen;
				m_recvRingBuffer.skipRead(t_iHasSearchLen);
				return true;
			}
			++t_iHasSearchLen;
		}

		if (m_recvRingBuffer.dataSize() > t_iHasSearchLen) {
			m_recvRingBuffer.skipRead(t_iHasSearchLen);
		}

		return false;
	}

} //namespace Related 