#include "jsonwrapper.h"

#include <QDateTime>

namespace CommonDefines {

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

	QByteArray JsonWrapper::wrapObject(std::function<void(QJsonObject &)> callback)
	{
		QJsonDocument doc;

		QJsonObject obj;
		callback(obj);
		doc.setObject(obj);

		return doc.toJson(QJsonDocument::Compact);
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

	QByteArray JsonWrapper::wrap(const Datastruct::UserLoginRequest & request)
	{
		return wrapObject([&](QJsonObject & obj) {
			obj.insert(m_jsonKey.name, request.m_name);
			obj.insert(m_jsonKey.password, request.m_password);
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::UserLoginRequest & request)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			request.m_name = jsonObject.value(m_jsonKey.name).toString();
			request.m_password = jsonObject.value(m_jsonKey.password).toString();
		});
	}

	QByteArray JsonWrapper::wrap(const Datastruct::UserLoginResponse & response)
	{
		return wrapObject([&](QJsonObject & obj) {
			obj.insert(m_jsonKey.result, response.m_loginResult);
			obj.insert(m_jsonKey.errorInfo, response.m_errorInfo);

			if (response.m_loginResult) {
				QJsonObject dataObj;

				dataObj.insert(m_jsonKey.id, response.m_userInfo.id);
				dataObj.insert(m_jsonKey.name, response.m_userInfo.name);
				dataObj.insert(m_jsonKey.password, response.m_userInfo.password);
				dataObj.insert(m_jsonKey.registTime, response.m_userInfo.registTime);
				dataObj.insert(m_jsonKey.privilege, response.m_userInfo.privilege);
				dataObj.insert(m_jsonKey.manager, response.m_userInfo.isManager);

				obj.insert(m_jsonKey.data, dataObj);
			}
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::UserLoginResponse & response)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			response.m_loginResult = jsonObject.value(m_jsonKey.result).toBool();
			response.m_errorInfo = jsonObject.value(m_jsonKey.errorInfo).toString();

			if (response.m_loginResult) {
				QJsonObject dataObj = jsonObject.value(m_jsonKey.data).toObject();
				if (dataObj.isEmpty())
					return;

				response.m_userInfo.id = dataObj.value(m_jsonKey.id).toInt();
				response.m_userInfo.name = dataObj.value(m_jsonKey.name).toString();
				response.m_userInfo.password = dataObj.value(m_jsonKey.password).toString();
				response.m_userInfo.registTime = dataObj.value(m_jsonKey.registTime).toString();
				response.m_userInfo.privilege = dataObj.value(m_jsonKey.privilege).toInt();
				response.m_userInfo.isManager = dataObj.value(m_jsonKey.manager).toBool();
			}
		});
	}

	QByteArray JsonWrapper::wrap(const Datastruct::UserRegistRequest & request)
	{
		return wrapObject([&](QJsonObject & obj) {
			obj.insert(m_jsonKey.name, request.m_name);
			obj.insert(m_jsonKey.password, request.m_password);
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::UserRegistRequest & request)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			request.m_name = jsonObject.value(m_jsonKey.name).toString();
			request.m_password = jsonObject.value(m_jsonKey.password).toString();
		});
	}

	QByteArray JsonWrapper::wrap(const Datastruct::UserRegistResponse & response)
	{
		return wrapObject([&](QJsonObject & obj) {
			obj.insert(m_jsonKey.result, response.m_loginResult);
			obj.insert(m_jsonKey.errorInfo, response.m_errorInfo);
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::UserRegistResponse & response)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			response.m_loginResult = jsonObject.value(m_jsonKey.result).toBool();
			response.m_errorInfo = jsonObject.value(m_jsonKey.errorInfo).toString();
		});
	}

} //namespace CommonDefines 