#include "jsonwrapper.h"

namespace Related {

	JsonWrapper * JsonWrapper::m_instance = nullptr;

	JsonWrapper::JsonWrapper(QObject *parent)
		: QObject(parent)
	{
	}

	bool JsonWrapper::unwrapObject(const QByteArray & data, std::function<void(QJsonObject &)> callback)
	{
		QJsonObject jsonObject = QJsonDocument::fromJson(data).object();

		if (jsonObject.isEmpty())
			return false;

		callback(jsonObject);

		return true;
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

	QByteArray JsonWrapper::wrap(Datastruct::PacketType type, const Datastruct::UserLoginRequest & request)
	{
		QJsonObject obj;
		obj.insert("name", request.m_name);
		obj.insert("password", request.m_password);

		QJsonDocument doc;
		doc.setObject(obj);

		return doc.toJson(QJsonDocument::Compact);
	}

	QByteArray JsonWrapper::wrap(Datastruct::PacketType type, const Datastruct::UserLoginResponse & response)
	{
		QJsonObject obj;
		obj.insert("result", response.m_loginResult);
		obj.insert("errorinfo", response.m_errorInfo);

		QJsonDocument doc;
		doc.setObject(obj);

		return doc.toJson(QJsonDocument::Compact);
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::UserLoginRequest & request)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			request.m_name = jsonObject.value("name").toString();
			request.m_password = jsonObject.value("password").toString();
			
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::UserLoginResponse & response)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			response.m_loginResult = jsonObject.value("result").toBool();
			response.m_errorInfo = jsonObject.value("errorinfo").toString();

		});
	}

} //namespace Related 