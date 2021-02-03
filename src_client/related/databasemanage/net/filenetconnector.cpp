#include "filenetconnector.h"

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

	void FileNetConnector::processNetData(QByteArray & data)
	{

	}

} //namespace Related 