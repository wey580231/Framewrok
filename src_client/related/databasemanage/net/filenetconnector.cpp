#include "filenetconnector.h"

#include <commondefines/wrapper/jsonwrapper.h>

#include <QDebug>

namespace Related {

	FileNetConnector * FileNetConnector::m_instance = nullptr;

	FileNetConnector::FileNetConnector()
		: NetConnector(Datastruct::File_Connection)
	{
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

	void FileNetConnector::write(Datastruct::FileInfoParameter  parameter, QByteArray data)
	{
		QByteArray array = makePacket(Datastruct::P_FileData, parameter, data);
		sendData(array);
	}

	void FileNetConnector::processNetData(QByteArray & data)
	{

	}

	QByteArray FileNetConnector::makePacket(Datastruct::PacketType type, Datastruct::FileInfoParameter  parameter, QByteArray data)
	{
		static int headLen = sizeof(Datastruct::PacketHead);
		static int parameterLen = sizeof(Datastruct::FileInfoParameter);
		static int tailLen = sizeof(Datastruct::PacketTail);

		Datastruct::PacketHead phead;
		phead.m_packetType = type;
		phead.m_dataLen = headLen + parameterLen + data.length() + tailLen;

		Datastruct::PacketTail ptail;

		QByteArray buff;
		buff.append((char *)&phead, headLen);
		buff.append((char *)&parameter, parameterLen);
		buff.append(data);
		buff.append((char *)&ptail, tailLen);

		return buff;
	}

} //namespace Related 