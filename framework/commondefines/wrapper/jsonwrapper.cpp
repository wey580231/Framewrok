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
				data.privilege = dataObj.value(m_jsonKey.privilege).toInt();
				data.isManager = dataObj.value(m_jsonKey.manager).toInt();

				response.m_userInfos.append(data);
			}

			response.m_userCount = jsonObject.value(m_jsonKey.totalDataSize).toInt();
		});
	}

	QByteArray JsonWrapper::wrap(const Datastruct::TaskCreateRequest & response)
	{
		return wrapObject([&](QJsonObject & obj) {
			obj.insert(m_jsonKey.id, response.taskId);
			obj.insert(m_jsonKey.name, response.taskName);
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::TaskCreateRequest & response)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			response.taskId = jsonObject.value(m_jsonKey.id).toString();
			response.taskName = jsonObject.value(m_jsonKey.name).toString();
		});
	}

	QByteArray JsonWrapper::wrap(const Datastruct::DutyRecordCreateRequest & request)
	{
		return wrapObject([&](QJsonObject & obj) {
			obj.insert(m_jsonKey.id, request.id);
			obj.insert(m_jsonKey.taskId, request.taskId);
			obj.insert(m_jsonKey.createTime, request.createTime);
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::DutyRecordCreateRequest & request)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			request.id = jsonObject.value(m_jsonKey.id).toString();
			request.taskId = jsonObject.value(m_jsonKey.taskId).toString();
			request.createTime = jsonObject.value(m_jsonKey.createTime).toString();
		});
	}

	QByteArray JsonWrapper::wrap(const Datastruct::DutyRecordCreateResponse & response)
	{
		return wrapObject([&](QJsonObject & obj) {
			obj.insert(m_jsonKey.result, response.m_createResult);
			obj.insert(m_jsonKey.errorInfo, response.m_errorInfo);

			if (response.m_createResult) {
				QJsonObject dataObj;

				dataObj.insert(m_jsonKey.id, response.dutyRecordInfo.id);
				dataObj.insert(m_jsonKey.taskId, response.dutyRecordInfo.taskId);
				dataObj.insert(m_jsonKey.createTime, response.dutyRecordInfo.createTime);
				obj.insert(m_jsonKey.data, dataObj);
			}
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::DutyRecordCreateResponse & response)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			response.m_createResult = jsonObject.value(m_jsonKey.result).toBool();
			response.m_errorInfo = jsonObject.value(m_jsonKey.errorInfo).toString();

			if (response.m_createResult) {
				QJsonObject dataObj = jsonObject.value(m_jsonKey.data).toObject();
				if (dataObj.isEmpty())
					return;

				response.dutyRecordInfo.id = dataObj.value(m_jsonKey.id).toString();
				response.dutyRecordInfo.taskId = dataObj.value(m_jsonKey.taskId).toString();
				response.dutyRecordInfo.createTime = dataObj.value(m_jsonKey.createTime).toString();

			}
		});
	}

	QByteArray JsonWrapper::wrap(const Datastruct::LoadAllDutyRecordRequest & request)
	{
		return wrapObject([&](QJsonObject & obj) {
			obj.insert(m_jsonKey.taskId, request.taskId);
			obj.insert(m_jsonKey.offsetIndex, request.m_offsetIndex);
			obj.insert(m_jsonKey.limitIndex, request.m_limitIndex);
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::LoadAllDutyRecordRequest & request)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			request.taskId = jsonObject.value(m_jsonKey.taskId).toString();
			request.m_offsetIndex = jsonObject.value(m_jsonKey.offsetIndex).toInt();
			request.m_limitIndex  = jsonObject.value(m_jsonKey.limitIndex).toInt();
		});
	}

	QByteArray JsonWrapper::wrap(const Datastruct::LoadAllDutyRecordResponse & response)
	{
		return wrapObject([&](QJsonObject & obj) {

			QJsonArray jarray;
			for (int i = 0; i < response.m_dutyRecordInfos.size(); i++) {
				const Datastruct::DutyRecordEntityData & dRdata = response.m_dutyRecordInfos.at(i);

				QJsonObject dataObj;

				dataObj.insert(m_jsonKey.id,			dRdata.id);
				dataObj.insert(m_jsonKey.taskId,		dRdata.taskId);
				dataObj.insert(m_jsonKey.createTime,	dRdata.createTime);

				jarray.append(dataObj);
			}
			obj.insert(m_jsonKey.totalDataSize, response.m_dutyRecordCount);
			obj.insert(m_jsonKey.data, jarray);
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::LoadAllDutyRecordResponse & response)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			QJsonArray jarray = jsonObject.value(m_jsonKey.data).toArray();
			for (int i = 0; i < jarray.size(); i++) {
				Datastruct::DutyRecordEntityData data;

				QJsonObject dataObj = jarray.at(i).toObject();

				data.id			= dataObj.value(m_jsonKey.id).toString();
				data.taskId		= dataObj.value(m_jsonKey.taskId).toString();
				data.createTime = dataObj.value(m_jsonKey.createTime).toString();

				response.m_dutyRecordInfos.append(data);
			}

			response.m_dutyRecordCount = jsonObject.value(m_jsonKey.totalDataSize).toInt();
		});
	}

	QByteArray JsonWrapper::wrap(const Datastruct::ExperimentRecordDeleteRequest & request)
	{
		return wrapObject([&](QJsonObject & obj) {
			obj.insert(m_jsonKey.id, request.id);
			obj.insert(m_jsonKey.taskId, request.taskId);
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::ExperimentRecordDeleteRequest & request)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			request.id = jsonObject.value(m_jsonKey.id).toString();
			request.taskId = jsonObject.value(m_jsonKey.taskId).toString();
		});
	}

	QByteArray JsonWrapper::wrap(const Datastruct::ExperimentRecordDeleteResponse & response)
	{
		return wrapObject([&](QJsonObject & obj) {
			obj.insert(m_jsonKey.result, response.m_deleteResult);
			obj.insert(m_jsonKey.errorInfo, response.m_errorInfo);
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::ExperimentRecordDeleteResponse & response)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			response.m_deleteResult = jsonObject.value(m_jsonKey.result).toBool();
			response.m_errorInfo = jsonObject.value(m_jsonKey.errorInfo).toString();
		});
	}

} //namespace CommonDefines 