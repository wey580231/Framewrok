#include "filenetconnector.h"

namespace Related {

	FileNetConnector * FileNetConnector::m_instance = nullptr;

	FileNetConnector::FileNetConnector()
		: NetConnector(Datastruct::File_Connection)
	{
	}

	QByteArray FileNetConnector::makePacket(Datastruct::PacketType type, QByteArray & body)
	{
		static int headLen = sizeof(Datastruct::PacketHead);
		static int tailLen = sizeof(Datastruct::PacketTail);

		Datastruct::PacketHead phead;
		phead.m_packetType = type;
		phead.m_dataLen = headLen + body.length() + tailLen;

		Datastruct::PacketTail ptail;

		QByteArray data;
		data.append((char *)&phead, headLen);
		data.append(body);
		data.append((char *)&ptail, tailLen);

		return data;
	}

	FileNetConnector * FileNetConnector::instance()
	{
		if (m_instance == nullptr)
			m_instance = new FileNetConnector();

		return m_instance;
	}

	FileNetConnector::~FileNetConnector()
	{
	}

	void FileNetConnector::write(QByteArray  buff)
	{
		QByteArray array = makePacket(Datastruct::P_RawFile, buff);

		sendData(array);
	}

	void FileNetConnector::processNetData(QByteArray & data)
	{

	}

} //namespace Related 