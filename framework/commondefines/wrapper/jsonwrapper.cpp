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
			obj.insert(m_jsonKey.errorInfo, response.m_errorCode);

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
			response.m_errorCode =  static_cast<Datastruct::ErrorCode>(jsonObject.value(m_jsonKey.errorInfo).toInt());

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
			obj.insert(m_jsonKey.errorInfo, response.m_errorCode);
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::UserRegistResponse & response)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			response.m_loginResult = jsonObject.value(m_jsonKey.result).toBool();
			response.m_errorCode = static_cast<Datastruct::ErrorCode>(jsonObject.value(m_jsonKey.errorInfo).toInt());
		});
	}

	QByteArray JsonWrapper::wrap(const Datastruct::LoadAllUserRequest & request)
	{
		return wrapObject([&](QJsonObject & obj) {
			obj.insert(m_jsonKey.name,request.m_name);
			obj.insert(m_jsonKey.offsetIndex, request.m_offsetIndex);
			obj.insert(m_jsonKey.limitIndex, request.m_limitIndex);
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::LoadAllUserRequest & request)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			request.m_name = jsonObject.value(m_jsonKey.name).toString();
			request.m_offsetIndex = jsonObject.value(m_jsonKey.offsetIndex).toInt();
			request.m_limitIndex = jsonObject.value(m_jsonKey.limitIndex).toInt();
		});
	}

	QByteArray JsonWrapper::wrap(const Datastruct::LoadAllUserResponse & response)
	{
		return wrapObject([&](QJsonObject & obj) {

			QJsonArray jarray;
			for (int i = 0; i < response.m_userInfos.size(); i++) {
				const Datastruct::UserEntityData & udata = response.m_userInfos.at(i);

				QJsonObject dataObj;

				dataObj.insert(m_jsonKey.id, udata.id);
				dataObj.insert(m_jsonKey.name, udata.name);
				dataObj.insert(m_jsonKey.registTime, udata.registTime);
				dataObj.insert(m_jsonKey.lastLoadTime, udata.lastLoadTime);
				dataObj.insert(m_jsonKey.privilege, udata.privilege);
				dataObj.insert(m_jsonKey.manager, udata.isManager);

				jarray.append(dataObj);
			}
			obj.insert(m_jsonKey.totalDataSize, response.m_userCount);
			obj.insert(m_jsonKey.data, jarray);
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::LoadAllUserResponse & response)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			QJsonArray jarray = jsonObject.value(m_jsonKey.data).toArray();
			for (int i = 0; i < jarray.size(); i++) {
				Datastruct::UserEntityData data;

				QJsonObject dataObj = jarray.at(i).toObject();
			
				data.id = dataObj.value(m_jsonKey.id).toInt();
				data.name = dataObj.value(m_jsonKey.name).toString();
				data.registTime = dataObj.value(m_jsonKey.registTime).toString();
				data.lastLoadTime = dataObj.value(m_jsonKey.lastLoadTime).toString();
				data.privilege = dataObj.value(m_jsonKey.privilege).toInt();
				data.isManager = dataObj.value(m_jsonKey.manager).toBool();

				response.m_userInfos.append(data);
			}

			response.m_userCount = jsonObject.value(m_jsonKey.totalDataSize).toInt();
		});
	}

	QByteArray JsonWrapper::wrap(const Datastruct::OperateUserRequest & request)
	{
		return wrapObject([&](QJsonObject & obj) {
			obj.insert(m_jsonKey.type, request.m_operateType);
			obj.insert(m_jsonKey.id, request.m_id);
			obj.insert(m_jsonKey.password, request.m_password);
			obj.insert(m_jsonKey.privilege, request.m_privilege);
			obj.insert(m_jsonKey.manageId, request.m_manageId);
			obj.insert(m_jsonKey.manager, request.m_isManage);
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::OperateUserRequest & request)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			request.m_operateType = static_cast<Datastruct::UserOperateType>(jsonObject.value(m_jsonKey.type).toInt());
			request.m_id = jsonObject.value(m_jsonKey.id).toInt();
			request.m_password = jsonObject.value(m_jsonKey.password).toString();
			request.m_privilege = jsonObject.value(m_jsonKey.privilege).toInt();
			request.m_manageId = jsonObject.value(m_jsonKey.manageId).toInt();
			request.m_isManage = jsonObject.value(m_jsonKey.manager).toBool();
		});
	}

	QByteArray JsonWrapper::wrap(const Datastruct::OperateUserResponse & response)
	{
		return wrapObject([&](QJsonObject & obj) {
			obj.insert(m_jsonKey.type, response.m_operateType);
			obj.insert(m_jsonKey.result, response.m_operateResult);
			obj.insert(m_jsonKey.errorInfo, response.m_errorCode);
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::OperateUserResponse & response)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			response.m_operateType = static_cast<Datastruct::UserOperateType>(jsonObject.value(m_jsonKey.type).toInt());
			response.m_operateResult = jsonObject.value(m_jsonKey.result).toBool();
			response.m_errorCode = static_cast<Datastruct::ErrorCode>(jsonObject.value(m_jsonKey.errorInfo).toInt());
		});
	}

} //namespace CommonDefines 