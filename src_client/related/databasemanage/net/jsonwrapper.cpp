#include "jsonwrapper.h"

namespace Related {

	JsonWrapper * JsonWrapper::m_instance = nullptr;

	JsonWrapper::JsonWrapper(QObject *parent)
		: QObject(parent)
	{
	}

	JsonWrapper * JsonWrapper::instance()
	{
		if (m_instance == nullptr)
			m_instance = new JsonWrapper();

		return m_instance;
	}

	JsonWrapper::~JsonWrapper()
	{
	}

	QByteArray JsonWrapper::wrap(PacketType type, const UserLoginRequest & request)
	{
		QJsonObject obj;
		obj.insert("name", request.m_name);
		obj.insert("password", request.m_password);
		
		QJsonDocument doc;
		doc.setObject(obj);

		return doc.toJson(QJsonDocument::Compact);
	}

} //namespace Related 