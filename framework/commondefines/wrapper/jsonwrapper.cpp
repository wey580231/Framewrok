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

	/***************************    有关用户     *********************************/
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

	/***************************    有关任务     *********************************/
	QByteArray JsonWrapper::wrap(const Datastruct::TaskCreateRequest & request)
	{
		return wrapObject([&](QJsonObject & obj) {
			obj.insert(m_jsonKey.id, request.m_taskId);
			obj.insert(m_jsonKey.name, request.m_taskName);
			obj.insert(m_jsonKey.startTime, request.m_startTime);
			obj.insert(m_jsonKey.endTime, request.m_endTime);
			obj.insert(m_jsonKey.location, request.m_location);
			obj.insert(m_jsonKey.lon, request.lon);
			obj.insert(m_jsonKey.lat, request.lat);
			obj.insert(m_jsonKey.description, request.description);
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::TaskCreateRequest & request)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			request.m_taskId = jsonObject.value(m_jsonKey.id).toString();
			request.m_taskName = jsonObject.value(m_jsonKey.name).toString();
			request.m_startTime = jsonObject.value(m_jsonKey.startTime).toString();
			request.m_endTime = jsonObject.value(m_jsonKey.endTime).toString();
			request.m_location = jsonObject.value(m_jsonKey.location).toString();
			request.lon = jsonObject.value(m_jsonKey.lon).toString();
			request.lat = jsonObject.value(m_jsonKey.lat).toString();
			request.description = jsonObject.value(m_jsonKey.description).toString();
		});
	}

	QByteArray JsonWrapper::wrap(const Datastruct::TaskCreateResponse & response)
	{
		return wrapObject([&](QJsonObject & obj) {
			obj.insert(m_jsonKey.result, response.m_createResult);
			obj.insert(m_jsonKey.errorInfo, response.m_errorInfo);

			if (response.m_createResult) {
				QJsonObject dataObj;

				dataObj.insert(m_jsonKey.id, response.taskInfo.id);
				dataObj.insert(m_jsonKey.name, response.taskInfo.taskName);
				dataObj.insert(m_jsonKey.startTime, response.taskInfo.startTime);
				dataObj.insert(m_jsonKey.endTime, response.taskInfo.endTime);
				dataObj.insert(m_jsonKey.location, response.taskInfo.location);
				dataObj.insert(m_jsonKey.lon, response.taskInfo.lon);
				dataObj.insert(m_jsonKey.lat, response.taskInfo.lat);
				dataObj.insert(m_jsonKey.description, response.taskInfo.description);
				obj.insert(m_jsonKey.data, dataObj);
			}
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::TaskCreateResponse & response)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			response.m_createResult = jsonObject.value(m_jsonKey.result).toBool();
			response.m_errorInfo = static_cast<Datastruct::ErrorCode>(jsonObject.value(m_jsonKey.errorInfo).toInt());

			if (response.m_createResult) {
				QJsonObject dataObj = jsonObject.value(m_jsonKey.data).toObject();
				if (dataObj.isEmpty())
					return;

				response.taskInfo.id = dataObj.value(m_jsonKey.id).toString();
				response.taskInfo.taskName = dataObj.value(m_jsonKey.name).toString();
				response.taskInfo.startTime = dataObj.value(m_jsonKey.startTime).toString();
				response.taskInfo.endTime = dataObj.value(m_jsonKey.endTime).toString();
				response.taskInfo.location = dataObj.value(m_jsonKey.location).toString();
				response.taskInfo.lon = dataObj.value(m_jsonKey.lon).toString();
				response.taskInfo.lat = dataObj.value(m_jsonKey.lat).toString();
				response.taskInfo.description = dataObj.value(m_jsonKey.description).toString();
			}
		});
	}

	QByteArray JsonWrapper::wrap(const Datastruct::LoadAllTaskRequest & request)
	{
		return wrapObject([&](QJsonObject & obj) {
			obj.insert(m_jsonKey.offsetIndex, request.m_offsetIndex);
			obj.insert(m_jsonKey.limitIndex, request.m_limitIndex);
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::LoadAllTaskRequest & request)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			request.m_offsetIndex = jsonObject.value(m_jsonKey.offsetIndex).toInt();
			request.m_limitIndex = jsonObject.value(m_jsonKey.limitIndex).toInt();
		});
	}

	QByteArray JsonWrapper::wrap(const Datastruct::LoadAllTaskResponse & response)
	{
		return wrapObject([&](QJsonObject & obj) {

			QJsonArray jarray;
			for (int i = 0; i < response.m_taskInfos.size(); i++) {
				const Datastruct::TaskEntityData & dRdata = response.m_taskInfos.at(i);

				QJsonObject dataObj;

				dataObj.insert(m_jsonKey.id,		dRdata.id);
				dataObj.insert(m_jsonKey.name,		dRdata.taskName);
				dataObj.insert(m_jsonKey.startTime, dRdata.startTime);
				dataObj.insert(m_jsonKey.endTime,	dRdata.endTime);
				dataObj.insert(m_jsonKey.location,	dRdata.location);
				dataObj.insert(m_jsonKey.lon, dRdata.lon);
				dataObj.insert(m_jsonKey.lat, dRdata.lat);
				dataObj.insert(m_jsonKey.description, dRdata.description);
				jarray.append(dataObj);
			}
			obj.insert(m_jsonKey.totalDataSize, response.m_count);
			obj.insert(m_jsonKey.data, jarray);
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::LoadAllTaskResponse & response)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			QJsonArray jarray = jsonObject.value(m_jsonKey.data).toArray();
			for (int i = 0; i < jarray.size(); i++) {
				Datastruct::TaskEntityData data;

				QJsonObject dataObj = jarray.at(i).toObject();

				data.id = dataObj.value(m_jsonKey.id).toString();
				data.taskName = dataObj.value(m_jsonKey.name).toString();
				data.startTime = dataObj.value(m_jsonKey.startTime).toString();
				data.endTime = dataObj.value(m_jsonKey.endTime).toString();
				data.location = dataObj.value(m_jsonKey.location).toString();
				data.lon	= dataObj.value(m_jsonKey.lon).toString();
				data.lat	= dataObj.value(m_jsonKey.lat).toString();
				data.description = dataObj.value(m_jsonKey.description).toString();

				response.m_taskInfos.append(data);
			}
			response.m_count = jsonObject.value(m_jsonKey.totalDataSize).toInt();
		});
	}

	QByteArray JsonWrapper::wrap(const Datastruct::TaskByConditionRequest & request)
	{
		return wrapObject([&](QJsonObject & obj) {
			obj.insert(m_jsonKey.startTime, request.startTime);
			obj.insert(m_jsonKey.endTime, request.endTime);
			obj.insert(m_jsonKey.location, request.location);
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::TaskByConditionRequest & request)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			request.startTime = jsonObject.value(m_jsonKey.startTime).toString();
			request.endTime = jsonObject.value(m_jsonKey.endTime).toString();
			request.location = jsonObject.value(m_jsonKey.location).toString();
		});
	}

	QByteArray JsonWrapper::wrap(const Datastruct::TaskByConditionResponse & response)
	{
		return wrapObject([&](QJsonObject & obj) {

			QJsonArray jarray;
			for (int i = 0; i < response.m_taskInfos.size(); i++) {
				const Datastruct::TaskEntityData & dRdata = response.m_taskInfos.at(i);

				QJsonObject dataObj;

				dataObj.insert(m_jsonKey.id, dRdata.id);
				dataObj.insert(m_jsonKey.name, dRdata.taskName);
				dataObj.insert(m_jsonKey.startTime, dRdata.startTime);
				dataObj.insert(m_jsonKey.endTime, dRdata.endTime);
				dataObj.insert(m_jsonKey.location, dRdata.location);
				dataObj.insert(m_jsonKey.lon, dRdata.lon);
				dataObj.insert(m_jsonKey.lat, dRdata.lat);
				dataObj.insert(m_jsonKey.description, dRdata.description);
				jarray.append(dataObj);
			}
			obj.insert(m_jsonKey.totalDataSize, response.m_count);
			obj.insert(m_jsonKey.data, jarray);
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::TaskByConditionResponse & response)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			QJsonArray jarray = jsonObject.value(m_jsonKey.data).toArray();
			for (int i = 0; i < jarray.size(); i++) {
				Datastruct::TaskEntityData data;

				QJsonObject dataObj = jarray.at(i).toObject();

				data.id = dataObj.value(m_jsonKey.id).toString();
				data.taskName = dataObj.value(m_jsonKey.name).toString();
				data.startTime = dataObj.value(m_jsonKey.startTime).toString();
				data.endTime = dataObj.value(m_jsonKey.endTime).toString();
				data.location = dataObj.value(m_jsonKey.location).toString();
				data.lon = dataObj.value(m_jsonKey.lon).toString();
				data.lat = dataObj.value(m_jsonKey.lat).toString();
				data.description = dataObj.value(m_jsonKey.description).toString();

				response.m_taskInfos.append(data);
			}
			response.m_count = jsonObject.value(m_jsonKey.totalDataSize).toInt();
		});
	}

	QByteArray JsonWrapper::wrap(const Datastruct::TaskDeleteRequest & request)
	{
		return wrapObject([&](QJsonObject & obj) {
			obj.insert(m_jsonKey.id, request.taskId);
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::TaskDeleteRequest & request)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			request.taskId = jsonObject.value(m_jsonKey.id).toString();
		});
	}

	QByteArray JsonWrapper::wrap(const Datastruct::TaskDeleteResponse & response)
	{
		return wrapObject([&](QJsonObject & obj) {
			obj.insert(m_jsonKey.result, response.m_deleteResult);
			obj.insert(m_jsonKey.errorInfo, response.m_errorInfo);
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::TaskDeleteResponse & response)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			response.m_deleteResult = jsonObject.value(m_jsonKey.result).toBool();
			response.m_errorInfo = static_cast<Datastruct::ErrorCode>(jsonObject.value(m_jsonKey.errorInfo).toInt());
		});
	}

	QByteArray JsonWrapper::wrap(const Datastruct::TaskStaticsInfoRequest & request)
	{
		return wrapObject([&](QJsonObject & obj) {
			obj.insert(m_jsonKey.id, request.id);
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::TaskStaticsInfoRequest & request)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			request.id = jsonObject.value(m_jsonKey.id).toString();
		});
	}

	QByteArray JsonWrapper::wrap(const Datastruct::TaskStaticsInfoResponse & response)
	{
		return wrapObject([&](QJsonObject & obj) {
			obj.insert(m_jsonKey.startTime, response.allTaskStartTime);
			obj.insert(m_jsonKey.endTime, response.allTaskEndTime);
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::TaskStaticsInfoResponse & response)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			response.allTaskStartTime = jsonObject.value(m_jsonKey.startTime).toString();
			response.allTaskEndTime = jsonObject.value(m_jsonKey.endTime).toString();
		});
		return false;
	}

	QByteArray JsonWrapper::wrap(const Datastruct::TaskSimpleRequest & request)
	{
		return wrapObject([&](QJsonObject & obj) {
			obj.insert(m_jsonKey.id, request.taskId);
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::TaskSimpleRequest & request)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			request.taskId = jsonObject.value(m_jsonKey.id).toString();
		});
	}

	QByteArray JsonWrapper::wrap(const Datastruct::TaskSimpleResponse & response)
	{
		return wrapObject([&](QJsonObject & obj) {
			obj.insert(m_jsonKey.result, response.m_result);
			obj.insert(m_jsonKey.errorInfo, response.m_errorInfo);

			if (response.m_result) {
				QJsonObject dataObj;

				dataObj.insert(m_jsonKey.id, response.taskInfo.id);
				dataObj.insert(m_jsonKey.name, response.taskInfo.taskName);
				dataObj.insert(m_jsonKey.startTime, response.taskInfo.startTime);
				dataObj.insert(m_jsonKey.endTime, response.taskInfo.endTime);
				dataObj.insert(m_jsonKey.location, response.taskInfo.location);
				dataObj.insert(m_jsonKey.lon, response.taskInfo.lon);
				dataObj.insert(m_jsonKey.lat, response.taskInfo.lat);
				dataObj.insert(m_jsonKey.description, response.taskInfo.description);
				obj.insert(m_jsonKey.data, dataObj);
			}
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::TaskSimpleResponse & response)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			response.m_result = jsonObject.value(m_jsonKey.result).toBool();
			response.m_errorInfo = static_cast<Datastruct::ErrorCode>(jsonObject.value(m_jsonKey.errorInfo).toInt());

			if (response.m_result) {
				QJsonObject dataObj = jsonObject.value(m_jsonKey.data).toObject();
				if (dataObj.isEmpty())
					return;

				response.taskInfo.id = dataObj.value(m_jsonKey.id).toString();
				response.taskInfo.taskName = dataObj.value(m_jsonKey.name).toString();
				response.taskInfo.startTime = dataObj.value(m_jsonKey.startTime).toString();
				response.taskInfo.endTime = dataObj.value(m_jsonKey.endTime).toString();
				response.taskInfo.location = dataObj.value(m_jsonKey.location).toString();
				response.taskInfo.lon = dataObj.value(m_jsonKey.lon).toString();
				response.taskInfo.lat = dataObj.value(m_jsonKey.lat).toString();
				response.taskInfo.description = dataObj.value(m_jsonKey.description).toString();
			}
		});
	}

	QByteArray JsonWrapper::wrap(const Datastruct::TaskModifyRequest & request)
	{
		return wrapObject([&](QJsonObject & obj) {
			obj.insert(m_jsonKey.id, request.taskId);
			obj.insert(m_jsonKey.name, request.taskName);
			obj.insert(m_jsonKey.startTime, request.startTime);
			obj.insert(m_jsonKey.endTime, request.endTime);
			obj.insert(m_jsonKey.location, request.location);
			obj.insert(m_jsonKey.lon, request.lon);
			obj.insert(m_jsonKey.lat, request.lat);
			obj.insert(m_jsonKey.description, request.description);
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::TaskModifyRequest & request)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			request.taskId = jsonObject.value(m_jsonKey.id).toString();
			request.taskName = jsonObject.value(m_jsonKey.name).toString();
			request.startTime = jsonObject.value(m_jsonKey.startTime).toString();
			request.endTime = jsonObject.value(m_jsonKey.endTime).toString();
			request.location = jsonObject.value(m_jsonKey.location).toString();
			request.lon = jsonObject.value(m_jsonKey.lon).toString();
			request.lat = jsonObject.value(m_jsonKey.lat).toString();
			request.description = jsonObject.value(m_jsonKey.description).toString();
		});
	}

	QByteArray JsonWrapper::wrap(const Datastruct::TaskModifyResponse & response)
	{
		return wrapObject([&](QJsonObject & obj) {
			obj.insert(m_jsonKey.result, response.m_result);
			obj.insert(m_jsonKey.errorInfo, response.m_errorInfo);

			if (response.m_result) {
				QJsonObject dataObj;

				dataObj.insert(m_jsonKey.id, response.taskInfo.id);
				dataObj.insert(m_jsonKey.name, response.taskInfo.taskName);
				dataObj.insert(m_jsonKey.startTime, response.taskInfo.startTime);
				dataObj.insert(m_jsonKey.endTime, response.taskInfo.endTime);
				dataObj.insert(m_jsonKey.location, response.taskInfo.location);
				dataObj.insert(m_jsonKey.lon, response.taskInfo.lon);
				dataObj.insert(m_jsonKey.lat, response.taskInfo.lat);
				dataObj.insert(m_jsonKey.description, response.taskInfo.description);
				obj.insert(m_jsonKey.data, dataObj);
			}
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::TaskModifyResponse & response)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			response.m_result = jsonObject.value(m_jsonKey.result).toBool();
			response.m_errorInfo = static_cast<Datastruct::ErrorCode>(jsonObject.value(m_jsonKey.errorInfo).toInt());

			if (response.m_result) {
				QJsonObject dataObj = jsonObject.value(m_jsonKey.data).toObject();
				if (dataObj.isEmpty())
					return;

				response.taskInfo.id = dataObj.value(m_jsonKey.id).toString();
				response.taskInfo.taskName = dataObj.value(m_jsonKey.name).toString();
				response.taskInfo.startTime = dataObj.value(m_jsonKey.startTime).toString();
				response.taskInfo.endTime = dataObj.value(m_jsonKey.endTime).toString();
				response.taskInfo.location = dataObj.value(m_jsonKey.location).toString();
				response.taskInfo.lon = dataObj.value(m_jsonKey.lon).toString();
				response.taskInfo.lat = dataObj.value(m_jsonKey.lat).toString();
				response.taskInfo.description = dataObj.value(m_jsonKey.description).toString();
			}
		});
	}

	/***************************    有关任务侦测平台     *********************************/
	QByteArray JsonWrapper::wrap(const Datastruct::TaskDetectPlatformCreateRequest & request)
	{
		return wrapObject([&](QJsonObject & obj) {
			obj.insert(m_jsonKey.id,		request.m_id);
			obj.insert(m_jsonKey.taskId,	request.m_taskId);
			obj.insert(m_jsonKey.detectId,	request.m_detectId);
			obj.insert(m_jsonKey.name,		request.m_name);
			obj.insert(m_jsonKey.sensorType,request.m_sensorType);
			obj.insert(m_jsonKey.platformPower, request.m_platformPower);
			obj.insert(m_jsonKey.startTime,	request.m_startTime);
			obj.insert(m_jsonKey.endTime,	request.m_endTime);
			obj.insert(m_jsonKey.lastTime,	request.m_lastTime);
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::TaskDetectPlatformCreateRequest & request)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			request.m_id		= jsonObject.value(m_jsonKey.id).toString();
			request.m_taskId	= jsonObject.value(m_jsonKey.taskId).toString();
			request.m_detectId	= jsonObject.value(m_jsonKey.detectId).toInt();
			request.m_name		= jsonObject.value(m_jsonKey.name).toString();
			request.m_sensorType = jsonObject.value(m_jsonKey.sensorType).toString();
			request.m_platformPower = jsonObject.value(m_jsonKey.platformPower).toInt();
			request.m_startTime = jsonObject.value(m_jsonKey.startTime).toString();
			request.m_endTime	= jsonObject.value(m_jsonKey.endTime).toString();
			request.m_lastTime	= jsonObject.value(m_jsonKey.lastTime).toInt();
		});
	}

	QByteArray JsonWrapper::wrap(const Datastruct::TaskDetectPlatformCreateResponse & response)
	{
		return wrapObject([&](QJsonObject & obj) {
			obj.insert(m_jsonKey.result, response.m_createResult);
			obj.insert(m_jsonKey.errorInfo, response.m_errorInfo);
			if (response.m_createResult) {
				QJsonObject dataObj;
				dataObj.insert(m_jsonKey.id,		response.m_dataInfo.id);
				dataObj.insert(m_jsonKey.taskId,	response.m_dataInfo.taskId);
				dataObj.insert(m_jsonKey.detectId,	response.m_dataInfo.detectId);
				dataObj.insert(m_jsonKey.name,		response.m_dataInfo.name);
				dataObj.insert(m_jsonKey.sensorType, response.m_dataInfo.sensorType);
				dataObj.insert(m_jsonKey.platformPower, response.m_dataInfo.platformPower);
				dataObj.insert(m_jsonKey.startTime, response.m_dataInfo.startTime);
				dataObj.insert(m_jsonKey.endTime,	response.m_dataInfo.endTime);
				dataObj.insert(m_jsonKey.lastTime,	response.m_dataInfo.lastTime);
				obj.insert(m_jsonKey.data, dataObj);
			}
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::TaskDetectPlatformCreateResponse & response)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			response.m_createResult = jsonObject.value(m_jsonKey.result).toBool();
			response.m_errorInfo = static_cast<Datastruct::ErrorCode>(jsonObject.value(m_jsonKey.errorInfo).toInt());
			if (response.m_createResult) {
				QJsonObject dataObj = jsonObject.value(m_jsonKey.data).toObject();
				if (dataObj.isEmpty())
					return;
				response.m_dataInfo.id			= dataObj.value(m_jsonKey.id).toString();
				response.m_dataInfo.taskId		= dataObj.value(m_jsonKey.taskId).toString();
				response.m_dataInfo.detectId	= dataObj.value(m_jsonKey.detectId).toInt();
				response.m_dataInfo.name		= dataObj.value(m_jsonKey.name).toString();
				response.m_dataInfo.sensorType	= dataObj.value(m_jsonKey.sensorType).toString();
				response.m_dataInfo.platformPower = dataObj.value(m_jsonKey.platformPower).toInt();
				response.m_dataInfo.startTime	= dataObj.value(m_jsonKey.startTime).toString();
				response.m_dataInfo.endTime		= dataObj.value(m_jsonKey.endTime).toString();
				response.m_dataInfo.lastTime	= dataObj.value(m_jsonKey.lastTime).toInt();
			}
		});
	}

	QByteArray JsonWrapper::wrap(const Datastruct::LoadAllTaskDetectPlatformRequest & request)
	{
		return wrapObject([&](QJsonObject & obj) {
			obj.insert(m_jsonKey.taskId, request.m_taskId);
			obj.insert(m_jsonKey.offsetIndex, request.m_offsetIndex);
			obj.insert(m_jsonKey.limitIndex, request.m_limitIndex);
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::LoadAllTaskDetectPlatformRequest & request)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			request.m_taskId = jsonObject.value(m_jsonKey.taskId).toString();
			request.m_offsetIndex = jsonObject.value(m_jsonKey.offsetIndex).toInt();
			request.m_limitIndex = jsonObject.value(m_jsonKey.limitIndex).toInt();
		});
	}

	QByteArray JsonWrapper::wrap(const Datastruct::LoadAllTaskDetectPlatformResponse & response)
	{
		return wrapObject([&](QJsonObject & obj) {
			QJsonArray jarray;
			for (int i = 0; i < response.m_dataInfos.size(); i++) {
				const Datastruct::TaskDetectPlatformEntityData & dRdata = response.m_dataInfos.at(i);
				QJsonObject dataObj;
				dataObj.insert(m_jsonKey.id,		dRdata.id);
				dataObj.insert(m_jsonKey.taskId,	dRdata.taskId);
				dataObj.insert(m_jsonKey.detectId,	dRdata.detectId);
				dataObj.insert(m_jsonKey.name,		dRdata.name);
				dataObj.insert(m_jsonKey.sensorType,dRdata.sensorType);
				dataObj.insert(m_jsonKey.platformPower, dRdata.platformPower);
				dataObj.insert(m_jsonKey.startTime, dRdata.startTime);
				dataObj.insert(m_jsonKey.endTime,	dRdata.endTime);
				dataObj.insert(m_jsonKey.lastTime,	dRdata.lastTime);
				jarray.append(dataObj);
			}
			obj.insert(m_jsonKey.totalDataSize, response.m_taskDetectPlatformCount);
			obj.insert(m_jsonKey.data, jarray);
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::LoadAllTaskDetectPlatformResponse & response)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			QJsonArray jarray = jsonObject.value(m_jsonKey.data).toArray();
			for (int i = 0; i < jarray.size(); i++) {
				Datastruct::TaskDetectPlatformEntityData data;

				QJsonObject dataObj = jarray.at(i).toObject();
				data.id			= dataObj.value(m_jsonKey.id).toString();
				data.taskId		= dataObj.value(m_jsonKey.taskId).toString();
				data.detectId	= dataObj.value(m_jsonKey.detectId).toInt();
				data.name		= dataObj.value(m_jsonKey.name).toString();
				data.sensorType = dataObj.value(m_jsonKey.sensorType).toString();
				data.platformPower = dataObj.value(m_jsonKey.platformPower).toInt();
				data.startTime	= dataObj.value(m_jsonKey.startTime).toString();
				data.endTime	= dataObj.value(m_jsonKey.endTime).toString();
				data.lastTime	= dataObj.value(m_jsonKey.lastTime).toInt();

				response.m_dataInfos.append(data);
			}
			response.m_taskDetectPlatformCount = jsonObject.value(m_jsonKey.totalDataSize).toInt();
		});
	}

	QByteArray JsonWrapper::wrap(const Datastruct::TaskDetectPlatformByConditionRequest & request)
	{
		return QByteArray();
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::TaskDetectPlatformByConditionRequest & request)
	{
		return false;
	}

	QByteArray JsonWrapper::wrap(const Datastruct::TaskDetectPlatformByConditionResponse & response)
	{
		return QByteArray();
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::TaskDetectPlatformByConditionResponse & response)
	{
		return false;
	}

	QByteArray JsonWrapper::wrap(const Datastruct::TaskDetectPlatformDeleteRequest & request)
	{
		return wrapObject([&](QJsonObject & obj) {
			obj.insert(m_jsonKey.id, request.m_id);
			obj.insert(m_jsonKey.taskId, request.m_taskId);
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::TaskDetectPlatformDeleteRequest & request)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			request.m_id = jsonObject.value(m_jsonKey.id).toString();
			request.m_taskId = jsonObject.value(m_jsonKey.taskId).toString();
		});
	}

	QByteArray JsonWrapper::wrap(const Datastruct::TaskDetectPlatformDeleteResponse & response)
	{
		return wrapObject([&](QJsonObject & obj) {
			obj.insert(m_jsonKey.result, response.m_deleteResult);
			obj.insert(m_jsonKey.errorInfo, response.m_errorInfo);
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::TaskDetectPlatformDeleteResponse & response)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			response.m_deleteResult = jsonObject.value(m_jsonKey.result).toBool();
			response.m_errorInfo = static_cast<Datastruct::ErrorCode>(jsonObject.value(m_jsonKey.errorInfo).toInt());
		});
	}

	QByteArray JsonWrapper::wrap(const Datastruct::TaskDetectPlatformModifyRequest & request)
	{
		return wrapObject([&](QJsonObject & obj) {
			obj.insert(m_jsonKey.id, request.m_id);
			obj.insert(m_jsonKey.taskId, request.m_taskId);
			obj.insert(m_jsonKey.detectId, request.m_detectId);
			obj.insert(m_jsonKey.name, request.m_name);
			obj.insert(m_jsonKey.sensorType, request.m_sensorType);
			obj.insert(m_jsonKey.platformPower, request.m_platformPower);
			obj.insert(m_jsonKey.startTime, request.m_startTime);
			obj.insert(m_jsonKey.endTime, request.m_endTime);
			obj.insert(m_jsonKey.lastTime, request.m_lastTime);
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::TaskDetectPlatformModifyRequest & request)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			request.m_id = jsonObject.value(m_jsonKey.id).toString();
			request.m_taskId = jsonObject.value(m_jsonKey.taskId).toString();
			request.m_detectId = jsonObject.value(m_jsonKey.detectId).toInt();
			request.m_name = jsonObject.value(m_jsonKey.name).toString();
			request.m_sensorType = jsonObject.value(m_jsonKey.sensorType).toString();
			request.m_platformPower = jsonObject.value(m_jsonKey.platformPower).toInt();
			request.m_startTime = jsonObject.value(m_jsonKey.startTime).toString();
			request.m_endTime = jsonObject.value(m_jsonKey.endTime).toString();
			request.m_lastTime = jsonObject.value(m_jsonKey.lastTime).toInt();
		});
	}

	QByteArray JsonWrapper::wrap(const Datastruct::TaskDetectPlatformModifyResponse & response)
	{
		return wrapObject([&](QJsonObject & obj) {
			obj.insert(m_jsonKey.result, response.m_modifyResult);
			obj.insert(m_jsonKey.errorInfo, response.m_errorInfo);
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::TaskDetectPlatformModifyResponse & response)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			response.m_modifyResult = jsonObject.value(m_jsonKey.result).toBool();
			response.m_errorInfo = static_cast<Datastruct::ErrorCode>(jsonObject.value(m_jsonKey.errorInfo).toInt());
		});
	}

	/******************************      任务试验图片资源        ******************************************/
	QByteArray JsonWrapper::wrap(const Datastruct::TaskImageCreateRequest & request)
	{
		return wrapObject([&](QJsonObject & obj) {
			obj.insert(m_jsonKey.id, request.m_id);
			obj.insert(m_jsonKey.taskId, request.m_taskId);
			obj.insert(m_jsonKey.realName, request.m_realName);
			obj.insert(m_jsonKey.suffix, request.m_suffix);
			obj.insert(m_jsonKey.uploadTime, request.m_uploadTime);
			obj.insert(m_jsonKey.imageSize, request.m_imageSize);
			obj.insert(m_jsonKey.description, request.m_description);
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::TaskImageCreateRequest & request)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			request.m_id = jsonObject.value(m_jsonKey.id).toString();
			request.m_taskId = jsonObject.value(m_jsonKey.taskId).toString();
			request.m_realName = jsonObject.value(m_jsonKey.realName).toString();
			request.m_suffix = jsonObject.value(m_jsonKey.suffix).toString();
			request.m_uploadTime = jsonObject.value(m_jsonKey.uploadTime).toString();
			request.m_imageSize = jsonObject.value(m_jsonKey.imageSize).toDouble();
			request.m_description = jsonObject.value(m_jsonKey.description).toString();
		});
	}

	QByteArray JsonWrapper::wrap(const Datastruct::TaskImageCreateResponse & response)
	{
		return wrapObject([&](QJsonObject & obj) {
			obj.insert(m_jsonKey.result, response.m_createResult);
			obj.insert(m_jsonKey.errorInfo, response.m_errorInfo);
			if (response.m_createResult) {
				QJsonObject dataObj;
				dataObj.insert(m_jsonKey.id, response.m_taskImageInfo.id);
				dataObj.insert(m_jsonKey.taskId, response.m_taskImageInfo.taskId);
				dataObj.insert(m_jsonKey.realName, response.m_taskImageInfo.realName);
				dataObj.insert(m_jsonKey.suffix, response.m_taskImageInfo.suffix);
				dataObj.insert(m_jsonKey.uploadTime, response.m_taskImageInfo.uploadTime);
				dataObj.insert(m_jsonKey.imageSize, response.m_taskImageInfo.imageSize);
				dataObj.insert(m_jsonKey.description, response.m_taskImageInfo.description);
				obj.insert(m_jsonKey.data, dataObj);
			}
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::TaskImageCreateResponse & response)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			response.m_createResult = jsonObject.value(m_jsonKey.result).toBool();
			response.m_errorInfo = static_cast<Datastruct::ErrorCode>(jsonObject.value(m_jsonKey.errorInfo).toInt());
			if (response.m_createResult) {
				QJsonObject dataObj = jsonObject.value(m_jsonKey.data).toObject();
				if (dataObj.isEmpty())
					return;
				response.m_taskImageInfo.id = dataObj.value(m_jsonKey.id).toString();
				response.m_taskImageInfo.taskId = dataObj.value(m_jsonKey.taskId).toString();
				response.m_taskImageInfo.realName = dataObj.value(m_jsonKey.realName).toString();
				response.m_taskImageInfo.suffix = dataObj.value(m_jsonKey.suffix).toString();
				response.m_taskImageInfo.uploadTime = dataObj.value(m_jsonKey.uploadTime).toString();
				response.m_taskImageInfo.imageSize = dataObj.value(m_jsonKey.imageSize).toDouble();
				response.m_taskImageInfo.description = dataObj.value(m_jsonKey.description).toString();
			}
		});
	}

	QByteArray JsonWrapper::wrap(const Datastruct::LoadAllTaskImageRequest & request)
	{
		return wrapObject([&](QJsonObject & obj) {
			obj.insert(m_jsonKey.taskId,		request.m_taskId);
			obj.insert(m_jsonKey.offsetIndex,	request.m_offsetIndex);
			obj.insert(m_jsonKey.limitIndex,	request.m_limitIndex);
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::LoadAllTaskImageRequest & request)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			request.m_taskId = jsonObject.value(m_jsonKey.taskId).toString();
			request.m_offsetIndex = jsonObject.value(m_jsonKey.offsetIndex).toInt();
			request.m_limitIndex = jsonObject.value(m_jsonKey.limitIndex).toInt();
		});
	}

	QByteArray JsonWrapper::wrap(const Datastruct::LoadAllTaskImageResponse & response)
	{
		return wrapObject([&](QJsonObject & obj) {
			QJsonArray jarray;
			for (int i = 0; i < response.m_taskImageInfos.size(); i++) {
				const Datastruct::TaskImageEntityData & dRdata = response.m_taskImageInfos.at(i);
				QJsonObject dataObj;
				dataObj.insert(m_jsonKey.id,		dRdata.id);
				dataObj.insert(m_jsonKey.taskId,	dRdata.taskId);
				dataObj.insert(m_jsonKey.realName,	dRdata.realName);
				dataObj.insert(m_jsonKey.suffix,	dRdata.suffix);
				dataObj.insert(m_jsonKey.uploadTime,dRdata.uploadTime);
				dataObj.insert(m_jsonKey.imageSize, dRdata.imageSize);
				dataObj.insert(m_jsonKey.description,dRdata.description);
				jarray.append(dataObj);
			}
			obj.insert(m_jsonKey.totalDataSize, response.m_taskImageCount);
			obj.insert(m_jsonKey.data, jarray);
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::LoadAllTaskImageResponse & response)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			QJsonArray jarray = jsonObject.value(m_jsonKey.data).toArray();
			for (int i = 0; i < jarray.size(); i++) {
				Datastruct::TaskImageEntityData data;
				QJsonObject dataObj = jarray.at(i).toObject();
				data.id = dataObj.value(m_jsonKey.id).toString();
				data.taskId = dataObj.value(m_jsonKey.taskId).toString();
				data.realName = dataObj.value(m_jsonKey.realName).toString();
				data.suffix = dataObj.value(m_jsonKey.suffix).toString();
				data.uploadTime = dataObj.value(m_jsonKey.uploadTime).toString();
				data.imageSize = dataObj.value(m_jsonKey.imageSize).toDouble();
				data.description = dataObj.value(m_jsonKey.description).toString();
				response.m_taskImageInfos.append(data);
			}
			response.m_taskImageCount = jsonObject.value(m_jsonKey.totalDataSize).toInt();
		});
	}

	QByteArray JsonWrapper::wrap(const Datastruct::TaskImageByConditionRequest & request)
	{
		return QByteArray();
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::TaskImageByConditionRequest & request)
	{
		return false;
	}

	QByteArray JsonWrapper::wrap(const Datastruct::TaskImageByConditionResponse & response)
	{
		return QByteArray();
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::TaskImageByConditionResponse & response)
	{
		return false;
	}

	QByteArray JsonWrapper::wrap(const Datastruct::TaskImageDeleteRequest & request)
	{
		return wrapObject([&](QJsonObject & obj) {
			obj.insert(m_jsonKey.id, request.m_id);
			obj.insert(m_jsonKey.taskId, request.m_taskId);
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::TaskImageDeleteRequest & request)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			request.m_id = jsonObject.value(m_jsonKey.id).toString();
			request.m_taskId = jsonObject.value(m_jsonKey.taskId).toString();
		});
	}

	QByteArray JsonWrapper::wrap(const Datastruct::TaskImageDeleteResponse & response)
	{
		return wrapObject([&](QJsonObject & obj) {
			obj.insert(m_jsonKey.result, response.m_deleteResult);
			obj.insert(m_jsonKey.errorInfo, response.m_errorInfo);
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::TaskImageDeleteResponse & response)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			response.m_deleteResult = jsonObject.value(m_jsonKey.result).toBool();
			response.m_errorInfo = static_cast<Datastruct::ErrorCode>(jsonObject.value(m_jsonKey.errorInfo).toInt());
		});
	}

	QByteArray JsonWrapper::wrap(const Datastruct::TaskImageModifyRequest & request)
	{
		return wrapObject([&](QJsonObject & obj) {
			obj.insert(m_jsonKey.id, request.m_id);
			obj.insert(m_jsonKey.taskId, request.m_taskId);
			obj.insert(m_jsonKey.realName, request.m_realName);
			obj.insert(m_jsonKey.suffix, request.m_suffix);
			obj.insert(m_jsonKey.uploadTime, request.m_uploadTime);
			obj.insert(m_jsonKey.imageSize, request.m_imageSize);
			obj.insert(m_jsonKey.description, request.m_description);
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::TaskImageModifyRequest & request)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			request.m_id = jsonObject.value(m_jsonKey.id).toString();
			request.m_taskId = jsonObject.value(m_jsonKey.taskId).toString();
			request.m_realName = jsonObject.value(m_jsonKey.realName).toString();
			request.m_suffix = jsonObject.value(m_jsonKey.suffix).toString();
			request.m_uploadTime = jsonObject.value(m_jsonKey.uploadTime).toString();
			request.m_imageSize = jsonObject.value(m_jsonKey.imageSize).toDouble();
			request.m_description = jsonObject.value(m_jsonKey.description).toString();
		});
	}

	QByteArray JsonWrapper::wrap(const Datastruct::TaskImageModifyResponse & response)
	{
		return wrapObject([&](QJsonObject & obj) {
			obj.insert(m_jsonKey.result, response.m_modifyResult);
			obj.insert(m_jsonKey.errorInfo, response.m_errorInfo);
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::TaskImageModifyResponse & response)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			response.m_modifyResult = jsonObject.value(m_jsonKey.result).toBool();
			response.m_errorInfo = static_cast<Datastruct::ErrorCode>(jsonObject.value(m_jsonKey.errorInfo).toInt());
		});
	}

	/******************************      值班日志        ******************************************/
	QByteArray JsonWrapper::wrap(const Datastruct::DutyRecordCreateRequest & request)
	{
		return wrapObject([&](QJsonObject & obj) {
			obj.insert(m_jsonKey.id,			request.m_id);
			obj.insert(m_jsonKey.taskId,		request.m_taskId);
			obj.insert(m_jsonKey.createTime,	request.m_createTime);
			obj.insert(m_jsonKey.description,	request.m_description);
			obj.insert(m_jsonKey.seaCondition,	request.m_seaCondition);
			obj.insert(m_jsonKey.wind,			request.m_wind);
			obj.insert(m_jsonKey.windSpeed,		request.m_windSpeed);
			obj.insert(m_jsonKey.waveHigh,		request.m_waveHigh);
			obj.insert(m_jsonKey.oceanCurrents,	request.m_oceanCurrents);
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::DutyRecordCreateRequest & request)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			request.m_id			= jsonObject.value(m_jsonKey.id).toString();
			request.m_taskId		= jsonObject.value(m_jsonKey.taskId).toString();
			request.m_createTime	= jsonObject.value(m_jsonKey.createTime).toString();
			request.m_description	= jsonObject.value(m_jsonKey.description).toString();
			request.m_seaCondition 	= jsonObject.value(m_jsonKey.seaCondition).toString();
			request.m_wind			= jsonObject.value(m_jsonKey.wind).toDouble();
			request.m_windSpeed		= jsonObject.value(m_jsonKey.windSpeed).toDouble();
			request.m_waveHigh		= jsonObject.value(m_jsonKey.waveHigh).toDouble();
			request.m_oceanCurrents = jsonObject.value(m_jsonKey.oceanCurrents).toDouble();
		});
	}

	QByteArray JsonWrapper::wrap(const Datastruct::DutyRecordCreateResponse & response)
	{
		return wrapObject([&](QJsonObject & obj) {
			obj.insert(m_jsonKey.result, response.m_createResult);
			obj.insert(m_jsonKey.errorInfo, response.m_errorInfo);

			if (response.m_createResult) {
				QJsonObject dataObj;
				dataObj.insert(m_jsonKey.id,			response.m_dutyRecordInfo.id);
				dataObj.insert(m_jsonKey.taskId,		response.m_dutyRecordInfo.taskId);
				dataObj.insert(m_jsonKey.createTime,	response.m_dutyRecordInfo.createTime);
				dataObj.insert(m_jsonKey.description,	response.m_dutyRecordInfo.description);
				dataObj.insert(m_jsonKey.seaCondition,	response.m_dutyRecordInfo.seaCondition);
				dataObj.insert(m_jsonKey.wind,			response.m_dutyRecordInfo.wind);
				dataObj.insert(m_jsonKey.windSpeed,		response.m_dutyRecordInfo.windSpeed);
				dataObj.insert(m_jsonKey.waveHigh,		response.m_dutyRecordInfo.waveHigh);
				dataObj.insert(m_jsonKey.oceanCurrents, response.m_dutyRecordInfo.oceanCurrents);
				obj.insert(m_jsonKey.data, dataObj);
			}
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::DutyRecordCreateResponse & response)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			response.m_createResult = jsonObject.value(m_jsonKey.result).toBool();
			response.m_errorInfo = static_cast<Datastruct::ErrorCode>(jsonObject.value(m_jsonKey.errorInfo).toInt());

			if (response.m_createResult) {
				QJsonObject dataObj = jsonObject.value(m_jsonKey.data).toObject();
				if (dataObj.isEmpty())
					return;

				response.m_dutyRecordInfo.id = jsonObject.value(m_jsonKey.id).toString();
				response.m_dutyRecordInfo.taskId = jsonObject.value(m_jsonKey.taskId).toString();
				response.m_dutyRecordInfo.createTime = jsonObject.value(m_jsonKey.createTime).toString();
				response.m_dutyRecordInfo.description = jsonObject.value(m_jsonKey.description).toString();
				response.m_dutyRecordInfo.seaCondition = jsonObject.value(m_jsonKey.seaCondition).toString();
				response.m_dutyRecordInfo.wind = jsonObject.value(m_jsonKey.wind).toDouble();
				response.m_dutyRecordInfo.windSpeed = jsonObject.value(m_jsonKey.windSpeed).toDouble();
				response.m_dutyRecordInfo.waveHigh = jsonObject.value(m_jsonKey.waveHigh).toDouble();
				response.m_dutyRecordInfo.oceanCurrents = jsonObject.value(m_jsonKey.oceanCurrents).toDouble();
			}
		});
	}

	QByteArray JsonWrapper::wrap(const Datastruct::LoadAllDutyRecordRequest & request)
	{
		return wrapObject([&](QJsonObject & obj) {
			obj.insert(m_jsonKey.taskId, request.m_taskId);
			obj.insert(m_jsonKey.offsetIndex, request.m_offsetIndex);
			obj.insert(m_jsonKey.limitIndex, request.m_limitIndex);
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::LoadAllDutyRecordRequest & request)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			request.m_taskId = jsonObject.value(m_jsonKey.taskId).toString();
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
				dataObj.insert(m_jsonKey.description,	dRdata.description);
				dataObj.insert(m_jsonKey.seaCondition,	dRdata.seaCondition);
				dataObj.insert(m_jsonKey.wind,			dRdata.wind);
				dataObj.insert(m_jsonKey.windSpeed,		dRdata.windSpeed);
				dataObj.insert(m_jsonKey.waveHigh,		dRdata.waveHigh);
				dataObj.insert(m_jsonKey.oceanCurrents, dRdata.oceanCurrents);
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

				data.id = dataObj.value(m_jsonKey.id).toString();
				data.taskId = dataObj.value(m_jsonKey.taskId).toString();
				data.createTime = dataObj.value(m_jsonKey.createTime).toString();
				data.description = dataObj.value(m_jsonKey.description).toString();
				data.seaCondition = dataObj.value(m_jsonKey.seaCondition).toString();
				data.wind = dataObj.value(m_jsonKey.wind).toDouble();
				data.windSpeed = dataObj.value(m_jsonKey.windSpeed).toDouble();
				data.waveHigh = dataObj.value(m_jsonKey.waveHigh).toDouble();
				data.oceanCurrents = dataObj.value(m_jsonKey.oceanCurrents).toDouble();

				response.m_dutyRecordInfos.append(data);
			}
			response.m_dutyRecordCount = jsonObject.value(m_jsonKey.totalDataSize).toInt();
		});
	}


	QByteArray JsonWrapper::wrap(const Datastruct::DutyRecordDeleteRequest & request)
	{
		return wrapObject([&](QJsonObject & obj) {
			obj.insert(m_jsonKey.id, request.m_id);
			obj.insert(m_jsonKey.taskId, request.m_taskId);
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::DutyRecordDeleteRequest & request)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			request.m_id = jsonObject.value(m_jsonKey.id).toString();
			request.m_taskId = jsonObject.value(m_jsonKey.taskId).toString();
		});
	}

	QByteArray JsonWrapper::wrap(const Datastruct::DutyRecordDeleteResponse & response)
	{
		return wrapObject([&](QJsonObject & obj) {
			obj.insert(m_jsonKey.result, response.m_deleteResult);
			obj.insert(m_jsonKey.errorInfo, response.m_errorInfo);
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::DutyRecordDeleteResponse & response)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			response.m_deleteResult = jsonObject.value(m_jsonKey.result).toBool();
			response.m_errorInfo = static_cast<Datastruct::ErrorCode>(jsonObject.value(m_jsonKey.errorInfo).toInt());
		});
	}

	QByteArray JsonWrapper::wrap(const Datastruct::DutyRecordModifyRequest & request)
	{
		return wrapObject([&](QJsonObject & obj) {
			obj.insert(m_jsonKey.id, request.m_id);
			obj.insert(m_jsonKey.taskId, request.m_taskId);
			obj.insert(m_jsonKey.createTime, request.m_createTime);
			obj.insert(m_jsonKey.description, request.m_description);
			obj.insert(m_jsonKey.seaCondition, request.m_seaCondition);
			obj.insert(m_jsonKey.wind, request.m_wind);
			obj.insert(m_jsonKey.windSpeed, request.m_windSpeed);
			obj.insert(m_jsonKey.waveHigh, request.m_waveHigh);
			obj.insert(m_jsonKey.oceanCurrents, request.m_oceanCurrents);
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::DutyRecordModifyRequest & request)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			request.m_id = jsonObject.value(m_jsonKey.id).toString();
			request.m_taskId = jsonObject.value(m_jsonKey.taskId).toString();
			request.m_createTime = jsonObject.value(m_jsonKey.createTime).toString();
			request.m_description = jsonObject.value(m_jsonKey.description).toString();
			request.m_seaCondition = jsonObject.value(m_jsonKey.seaCondition).toString();
			request.m_wind = jsonObject.value(m_jsonKey.wind).toDouble();
			request.m_windSpeed = jsonObject.value(m_jsonKey.windSpeed).toDouble();
			request.m_waveHigh = jsonObject.value(m_jsonKey.waveHigh).toDouble();
			request.m_oceanCurrents = jsonObject.value(m_jsonKey.oceanCurrents).toDouble();
		});
	}

	QByteArray JsonWrapper::wrap(const Datastruct::DutyRecordModifyResponse & response)
	{
		return wrapObject([&](QJsonObject & obj) {
			obj.insert(m_jsonKey.result, response.m_modifyResult);
			obj.insert(m_jsonKey.errorInfo, response.m_errorInfo);
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::DutyRecordModifyResponse & response)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			response.m_modifyResult = jsonObject.value(m_jsonKey.result).toBool();
			response.m_errorInfo = static_cast<Datastruct::ErrorCode>(jsonObject.value(m_jsonKey.errorInfo).toInt());
		});
	}

	/********************************  试验记录  ********************************/
	QByteArray JsonWrapper::wrap(const Datastruct::ExperimentRecordCreateRequest & request)
	{
		return wrapObject([&](QJsonObject & obj) {
			obj.insert(m_jsonKey.id, request.m_id);
			obj.insert(m_jsonKey.taskId, request.m_taskId);
			obj.insert(m_jsonKey.platformId, request.m_platformId);
			obj.insert(m_jsonKey.floatingTime, request.m_floatingTime);
			obj.insert(m_jsonKey.lon, request.m_lon);
			obj.insert(m_jsonKey.lat, request.m_lat);
			obj.insert(m_jsonKey.setHeadingDegree, request.m_setHeadingDegree);
			obj.insert(m_jsonKey.actualHeadingDegree, request.m_actualHeadingDegree);
			obj.insert(m_jsonKey.acousticState, request.m_acousticState);
			obj.insert(m_jsonKey.targetNum, request.m_targetNum);
			obj.insert(m_jsonKey.underwaterTargetNum, request.m_underwaterTargetNum);
			obj.insert(m_jsonKey.underwaterTargetInfo, request.m_underwaterTargetInfo);
			obj.insert(m_jsonKey.maxDepth, request.m_maxDepth);
			obj.insert(m_jsonKey.profileIndex, request.m_profileIndex);
			obj.insert(m_jsonKey.profileLength, request.m_profileLength);
			obj.insert(m_jsonKey.profileDistance, request.m_profileDistance);
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::ExperimentRecordCreateRequest & request)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			request.m_id = jsonObject.value(m_jsonKey.id).toString();
			request.m_taskId = jsonObject.value(m_jsonKey.taskId).toString();
			request.m_platformId = jsonObject.value(m_jsonKey.platformId).toString();
			request.m_floatingTime = jsonObject.value(m_jsonKey.floatingTime).toString();
			request.m_lon = jsonObject.value(m_jsonKey.lon).toDouble();
			request.m_lat = jsonObject.value(m_jsonKey.lat).toDouble();
			request.m_setHeadingDegree = jsonObject.value(m_jsonKey.setHeadingDegree).toDouble();
			request.m_actualHeadingDegree = jsonObject.value(m_jsonKey.actualHeadingDegree).toDouble();
			request.m_acousticState = jsonObject.value(m_jsonKey.acousticState).toInt();
			request.m_targetNum = jsonObject.value(m_jsonKey.targetNum).toInt();
			request.m_underwaterTargetNum = jsonObject.value(m_jsonKey.underwaterTargetNum).toInt();
			request.m_underwaterTargetInfo = jsonObject.value(m_jsonKey.underwaterTargetInfo).toString();
			request.m_maxDepth = jsonObject.value(m_jsonKey.maxDepth).toDouble();
			request.m_profileIndex = jsonObject.value(m_jsonKey.profileIndex).toInt();
			request.m_profileLength = jsonObject.value(m_jsonKey.profileLength).toDouble();
			request.m_profileDistance = jsonObject.value(m_jsonKey.profileDistance).toDouble();
		});
	}

	QByteArray JsonWrapper::wrap(const Datastruct::ExperimentRecordCreateResponse & response)
	{
		return wrapObject([&](QJsonObject & obj) {
			obj.insert(m_jsonKey.result, response.m_createResult);
			obj.insert(m_jsonKey.errorInfo, response.m_errorInfo);

			if (response.m_createResult) {
				QJsonObject dataObj;

				dataObj.insert(m_jsonKey.id, response.m_experimentRecordInfo.id);
				dataObj.insert(m_jsonKey.taskId, response.m_experimentRecordInfo.taskId);
				dataObj.insert(m_jsonKey.platformId, response.m_experimentRecordInfo.platformId);
				dataObj.insert(m_jsonKey.floatingTime, response.m_experimentRecordInfo.floatingTime);
				dataObj.insert(m_jsonKey.lon, response.m_experimentRecordInfo.lon);
				dataObj.insert(m_jsonKey.lat, response.m_experimentRecordInfo.lat);
				dataObj.insert(m_jsonKey.setHeadingDegree, response.m_experimentRecordInfo.setHeadingDegree);
				dataObj.insert(m_jsonKey.actualHeadingDegree, response.m_experimentRecordInfo.actualHeadingDegree);
				dataObj.insert(m_jsonKey.acousticState, response.m_experimentRecordInfo.acousticState);
				dataObj.insert(m_jsonKey.targetNum, response.m_experimentRecordInfo.targetNum);
				dataObj.insert(m_jsonKey.underwaterTargetNum, response.m_experimentRecordInfo.underwaterTargetNum);
				dataObj.insert(m_jsonKey.underwaterTargetInfo, response.m_experimentRecordInfo.underwaterTargetInfo);
				dataObj.insert(m_jsonKey.maxDepth, response.m_experimentRecordInfo.maxDepth);
				dataObj.insert(m_jsonKey.profileIndex, response.m_experimentRecordInfo.profileIndex);
				dataObj.insert(m_jsonKey.profileLength, response.m_experimentRecordInfo.profileLength);
				dataObj.insert(m_jsonKey.profileDistance, response.m_experimentRecordInfo.profileDistance);

				obj.insert(m_jsonKey.data, dataObj);
			}
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::ExperimentRecordCreateResponse & response)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			response.m_createResult = jsonObject.value(m_jsonKey.result).toBool();
			response.m_errorInfo = static_cast<Datastruct::ErrorCode>(jsonObject.value(m_jsonKey.errorInfo).toInt());

			if (response.m_createResult) {
				QJsonObject dataObj = jsonObject.value(m_jsonKey.data).toObject();
				if (dataObj.isEmpty())
					return;
				response.m_experimentRecordInfo.id = jsonObject.value(m_jsonKey.id).toString();
				response.m_experimentRecordInfo.taskId = jsonObject.value(m_jsonKey.taskId).toString();
				response.m_experimentRecordInfo.platformId = jsonObject.value(m_jsonKey.platformId).toString();
				response.m_experimentRecordInfo.floatingTime = jsonObject.value(m_jsonKey.floatingTime).toString();
				response.m_experimentRecordInfo.lon = jsonObject.value(m_jsonKey.lon).toDouble();
				response.m_experimentRecordInfo.lat = jsonObject.value(m_jsonKey.lat).toDouble();
				response.m_experimentRecordInfo.setHeadingDegree = jsonObject.value(m_jsonKey.setHeadingDegree).toDouble();
				response.m_experimentRecordInfo.actualHeadingDegree = jsonObject.value(m_jsonKey.actualHeadingDegree).toDouble();
				response.m_experimentRecordInfo.acousticState = jsonObject.value(m_jsonKey.acousticState).toInt();
				response.m_experimentRecordInfo.targetNum = jsonObject.value(m_jsonKey.targetNum).toInt();
				response.m_experimentRecordInfo.underwaterTargetNum = jsonObject.value(m_jsonKey.underwaterTargetNum).toInt();
				response.m_experimentRecordInfo.underwaterTargetInfo = jsonObject.value(m_jsonKey.underwaterTargetInfo).toString();
				response.m_experimentRecordInfo.maxDepth = jsonObject.value(m_jsonKey.maxDepth).toDouble();
				response.m_experimentRecordInfo.profileIndex = jsonObject.value(m_jsonKey.profileIndex).toInt();
				response.m_experimentRecordInfo.profileLength = jsonObject.value(m_jsonKey.profileLength).toDouble();
				response.m_experimentRecordInfo.profileDistance = jsonObject.value(m_jsonKey.profileDistance).toDouble();
			}
		});
	}

	QByteArray JsonWrapper::wrap(const Datastruct::LoadAllExperimentRecordsRequest & request)
	{
		return wrapObject([&](QJsonObject & obj) {
			obj.insert(m_jsonKey.taskId, request.m_taskId);
			obj.insert(m_jsonKey.platformId, request.m_platformId);
			obj.insert(m_jsonKey.offsetIndex, request.m_offsetIndex);
			obj.insert(m_jsonKey.limitIndex, request.m_limitIndex);
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::LoadAllExperimentRecordsRequest & request)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			request.m_taskId = jsonObject.value(m_jsonKey.taskId).toString();
			request.m_platformId = jsonObject.value(m_jsonKey.platformId).toString();
			request.m_offsetIndex = jsonObject.value(m_jsonKey.offsetIndex).toInt();
			request.m_limitIndex = jsonObject.value(m_jsonKey.limitIndex).toInt();
		});
	}

	QByteArray JsonWrapper::wrap(const Datastruct::LoadAllExperimentRecordsResponse & response)
	{
		return wrapObject([&](QJsonObject & obj) {

			QJsonArray jarray;
			for (int i = 0; i < response.m_experimentRecordInfos.size(); i++) {
				const Datastruct::ExperimentRecordEntityData & dRdata = response.m_experimentRecordInfos.at(i);

				QJsonObject dataObj;

				dataObj.insert(m_jsonKey.id,					dRdata.id);
				dataObj.insert(m_jsonKey.taskId,				dRdata.taskId);
				dataObj.insert(m_jsonKey.platformId,			dRdata.platformId);
				dataObj.insert(m_jsonKey.floatingTime,			dRdata.floatingTime);
				dataObj.insert(m_jsonKey.lon,					dRdata.lon);
				dataObj.insert(m_jsonKey.lat,					dRdata.lat);
				dataObj.insert(m_jsonKey.setHeadingDegree,		dRdata.setHeadingDegree);
				dataObj.insert(m_jsonKey.actualHeadingDegree,	dRdata.actualHeadingDegree);
				dataObj.insert(m_jsonKey.acousticState,			dRdata.acousticState);
				dataObj.insert(m_jsonKey.targetNum,				dRdata.targetNum);
				dataObj.insert(m_jsonKey.underwaterTargetNum,	dRdata.underwaterTargetNum);
				dataObj.insert(m_jsonKey.underwaterTargetInfo,	dRdata.underwaterTargetInfo);
				dataObj.insert(m_jsonKey.maxDepth,				dRdata.maxDepth);
				dataObj.insert(m_jsonKey.profileIndex,			dRdata.profileIndex);
				dataObj.insert(m_jsonKey.profileLength,			dRdata.profileLength);
				dataObj.insert(m_jsonKey.profileDistance,		dRdata.profileDistance);

				jarray.append(dataObj);
			}
			obj.insert(m_jsonKey.totalDataSize, response.m_experimentRecordCount);
			obj.insert(m_jsonKey.data, jarray);
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::LoadAllExperimentRecordsResponse & response)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			QJsonArray jarray = jsonObject.value(m_jsonKey.data).toArray();
			for (int i = 0; i < jarray.size(); i++) {
				Datastruct::ExperimentRecordEntityData data;

				QJsonObject dataObj = jarray.at(i).toObject();

				data.id = dataObj.value(m_jsonKey.id).toString();
				data.taskId = dataObj.value(m_jsonKey.taskId).toString();
				data.platformId = dataObj.value(m_jsonKey.platformId).toString();
				data.floatingTime = dataObj.value(m_jsonKey.floatingTime).toString();
				data.lon = dataObj.value(m_jsonKey.lon).toDouble();
				data.lat = dataObj.value(m_jsonKey.lat).toDouble();
				data.setHeadingDegree = dataObj.value(m_jsonKey.setHeadingDegree).toDouble();
				data.actualHeadingDegree = dataObj.value(m_jsonKey.actualHeadingDegree).toDouble();
				data.acousticState = dataObj.value(m_jsonKey.acousticState).toInt();
				data.targetNum = dataObj.value(m_jsonKey.targetNum).toInt();
				data.underwaterTargetNum = dataObj.value(m_jsonKey.underwaterTargetNum).toInt();
				data.underwaterTargetInfo = dataObj.value(m_jsonKey.underwaterTargetInfo).toString();
				data.maxDepth = dataObj.value(m_jsonKey.maxDepth).toDouble();
				data.profileIndex = dataObj.value(m_jsonKey.profileIndex).toInt();
				data.profileLength = dataObj.value(m_jsonKey.profileLength).toDouble();
				data.profileDistance = dataObj.value(m_jsonKey.profileDistance).toDouble();

				response.m_experimentRecordInfos.append(data);
			}

			response.m_experimentRecordCount = jsonObject.value(m_jsonKey.totalDataSize).toInt();
		});
	}

	QByteArray JsonWrapper::wrap(const Datastruct::ExperimentRecordDeleteRequest & request)
	{
		return wrapObject([&](QJsonObject & obj) {
			obj.insert(m_jsonKey.id, request.m_id);
			obj.insert(m_jsonKey.taskId, request.m_taskId);
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::ExperimentRecordDeleteRequest & request)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			request.m_id = jsonObject.value(m_jsonKey.id).toString();
			request.m_taskId = jsonObject.value(m_jsonKey.taskId).toString();
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
			response.m_errorInfo = static_cast<Datastruct::ErrorCode>(jsonObject.value(m_jsonKey.errorInfo).toInt());
		});
	}

	QByteArray JsonWrapper::wrap(const Datastruct::ExperimentRecordModifyRequest & request)
	{
		return wrapObject([&](QJsonObject & obj) {
			obj.insert(m_jsonKey.id, request.m_id);
			obj.insert(m_jsonKey.taskId, request.m_taskId);
			obj.insert(m_jsonKey.platformId, request.m_platformId);
			obj.insert(m_jsonKey.floatingTime, request.m_floatingTime);
			obj.insert(m_jsonKey.lon, request.m_lon);
			obj.insert(m_jsonKey.lat, request.m_lat);
			obj.insert(m_jsonKey.setHeadingDegree, request.m_setHeadingDegree);
			obj.insert(m_jsonKey.actualHeadingDegree, request.m_actualHeadingDegree);
			obj.insert(m_jsonKey.acousticState, request.m_acousticState);
			obj.insert(m_jsonKey.targetNum, request.m_targetNum);
			obj.insert(m_jsonKey.underwaterTargetNum, request.m_underwaterTargetNum);
			obj.insert(m_jsonKey.underwaterTargetInfo, request.m_underwaterTargetInfo);
			obj.insert(m_jsonKey.maxDepth, request.m_maxDepth);
			obj.insert(m_jsonKey.profileIndex, request.m_profileIndex);
			obj.insert(m_jsonKey.profileLength, request.m_profileLength);
			obj.insert(m_jsonKey.profileDistance, request.m_profileDistance);
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::ExperimentRecordModifyRequest & request)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			request.m_id = jsonObject.value(m_jsonKey.id).toString();
			request.m_taskId = jsonObject.value(m_jsonKey.taskId).toString();
			request.m_platformId = jsonObject.value(m_jsonKey.platformId).toString();
			request.m_floatingTime = jsonObject.value(m_jsonKey.floatingTime).toString();
			request.m_lon = jsonObject.value(m_jsonKey.lon).toDouble();
			request.m_lat = jsonObject.value(m_jsonKey.lat).toDouble();
			request.m_setHeadingDegree = jsonObject.value(m_jsonKey.setHeadingDegree).toDouble();
			request.m_actualHeadingDegree = jsonObject.value(m_jsonKey.actualHeadingDegree).toDouble();
			request.m_acousticState = jsonObject.value(m_jsonKey.acousticState).toInt();
			request.m_targetNum = jsonObject.value(m_jsonKey.targetNum).toInt();
			request.m_underwaterTargetNum = jsonObject.value(m_jsonKey.underwaterTargetNum).toInt();
			request.m_underwaterTargetInfo = jsonObject.value(m_jsonKey.underwaterTargetInfo).toString();
			request.m_maxDepth = jsonObject.value(m_jsonKey.maxDepth).toDouble();
			request.m_profileIndex = jsonObject.value(m_jsonKey.profileIndex).toInt();
			request.m_profileLength = jsonObject.value(m_jsonKey.profileLength).toDouble();
			request.m_profileDistance = jsonObject.value(m_jsonKey.profileDistance).toDouble();
		});
	}

	QByteArray JsonWrapper::wrap(const Datastruct::ExperimentRecordModifyResponse & response)
	{
		return wrapObject([&](QJsonObject & obj) {
			obj.insert(m_jsonKey.result, response.m_modifyResult);
			obj.insert(m_jsonKey.errorInfo, response.m_errorInfo);
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::ExperimentRecordModifyResponse & response)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			response.m_modifyResult = jsonObject.value(m_jsonKey.result).toBool();
			response.m_errorInfo = static_cast<Datastruct::ErrorCode>(jsonObject.value(m_jsonKey.errorInfo).toInt());
		});
	}

	QByteArray JsonWrapper::wrap(const Datastruct::DetectPlatformCreateRequest & request)
	{
		return wrapObject([&](QJsonObject & obj) {
			obj.insert(m_jsonKey.id, request.m_id);
			obj.insert(m_jsonKey.name, request.m_name);
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::DetectPlatformCreateRequest & request)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			request.m_id = jsonObject.value(m_jsonKey.id).toInt();
			request.m_name = jsonObject.value(m_jsonKey.name).toString();
		});
	}

	QByteArray JsonWrapper::wrap(const Datastruct::DetectPlatformCreateResponse & response)
	{
		return wrapObject([&](QJsonObject & obj) {
			obj.insert(m_jsonKey.result, response.m_createResult);
			obj.insert(m_jsonKey.errorInfo, response.m_errorInfo);

			if (response.m_createResult) {
				QJsonObject dataObj;
				dataObj.insert(m_jsonKey.id, response.m_detectPlatformInfo.id);
				dataObj.insert(m_jsonKey.name, response.m_detectPlatformInfo.name);
				obj.insert(m_jsonKey.data, dataObj);
			}
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::DetectPlatformCreateResponse & response)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			response.m_createResult = jsonObject.value(m_jsonKey.result).toBool();
			response.m_errorInfo = static_cast<Datastruct::ErrorCode>(jsonObject.value(m_jsonKey.errorInfo).toInt());

			if (response.m_createResult) {
				QJsonObject dataObj = jsonObject.value(m_jsonKey.data).toObject();
				if (dataObj.isEmpty())
					return;
				response.m_detectPlatformInfo.id = jsonObject.value(m_jsonKey.id).toInt();
				response.m_detectPlatformInfo.name = jsonObject.value(m_jsonKey.name).toString();
			}
		});
	}

	QByteArray JsonWrapper::wrap(const Datastruct::LoadAllDetectPlatformsRequest & request)
	{
		return wrapObject([&](QJsonObject & obj) {
			obj.insert(m_jsonKey.limitIndex, request.m_limitIndex);
			obj.insert(m_jsonKey.offsetIndex, request.m_offsetIndex);
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::LoadAllDetectPlatformsRequest & request)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			request.m_limitIndex = jsonObject.value(m_jsonKey.limitIndex).toInt();
			request.m_offsetIndex = jsonObject.value(m_jsonKey.offsetIndex).toInt();
		});
	}

	QByteArray JsonWrapper::wrap(const Datastruct::LoadAllDetectPlatformsResponse & response)
	{
		return wrapObject([&](QJsonObject & obj) {
			QJsonArray jarray;
			for (int i = 0; i < response.m_detectPlatformInfos.size(); i++) {
				const Datastruct::DetectPlatformEntityData & dRdata = response.m_detectPlatformInfos.at(i);

				QJsonObject dataObj;
				dataObj.insert(m_jsonKey.id, dRdata.id);
				dataObj.insert(m_jsonKey.name, dRdata.name);
				jarray.append(dataObj);
			}
			obj.insert(m_jsonKey.totalDataSize, response.m_detectPlatformCount);
			obj.insert(m_jsonKey.data, jarray);
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::LoadAllDetectPlatformsResponse & response)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			QJsonArray jarray = jsonObject.value(m_jsonKey.data).toArray();
			for (int i = 0; i < jarray.size(); i++) {
				Datastruct::DetectPlatformEntityData data;
				QJsonObject dataObj = jarray.at(i).toObject();
				data.id = dataObj.value(m_jsonKey.id).toInt();
				data.name = dataObj.value(m_jsonKey.name).toString();
				response.m_detectPlatformInfos.append(data);
			}
			response.m_detectPlatformCount = jsonObject.value(m_jsonKey.totalDataSize).toInt();
		});
	}

	QByteArray JsonWrapper::wrap(const Datastruct::DetectPlatformDeleteRequest & request)
	{
		return wrapObject([&](QJsonObject & obj) {
			obj.insert(m_jsonKey.id, request.m_id);
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::DetectPlatformDeleteRequest & request)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			request.m_id = jsonObject.value(m_jsonKey.id).toInt();
		});
	}

	QByteArray JsonWrapper::wrap(const Datastruct::DetectPlatformDeleteResponse & response)
	{
		return wrapObject([&](QJsonObject & obj) {
			obj.insert(m_jsonKey.result, response.m_deleteResult);
			obj.insert(m_jsonKey.errorInfo, response.m_errorInfo);
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::DetectPlatformDeleteResponse & response)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			response.m_deleteResult = jsonObject.value(m_jsonKey.result).toBool();
			response.m_errorInfo = static_cast<Datastruct::ErrorCode>(jsonObject.value(m_jsonKey.errorInfo).toInt());
		});
	}

	QByteArray JsonWrapper::wrap(const Datastruct::DetectPlatformModifyRequest & request)
	{
		return wrapObject([&](QJsonObject & obj) {
			obj.insert(m_jsonKey.id, request.m_id);
			obj.insert(m_jsonKey.name, request.m_name);
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::DetectPlatformModifyRequest & request)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			request.m_id = jsonObject.value(m_jsonKey.id).toInt();
			request.m_name = jsonObject.value(m_jsonKey.name).toString();
		});
	}

	QByteArray JsonWrapper::wrap(const Datastruct::DetectPlatformModifyResponse & response)
	{
		return wrapObject([&](QJsonObject & obj) {
			obj.insert(m_jsonKey.result, response.m_modifyResult);
			obj.insert(m_jsonKey.errorInfo, response.m_errorInfo);
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::DetectPlatformModifyResponse & response)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			response.m_modifyResult = jsonObject.value(m_jsonKey.result).toBool();
			response.m_errorInfo = static_cast<Datastruct::ErrorCode>(jsonObject.value(m_jsonKey.errorInfo).toInt());
		});
	}

	QByteArray JsonWrapper::wrap(const Datastruct::DetectPlatformSubtypeCreateRequest & request)
	{
		return wrapObject([&](QJsonObject & obj) {
			obj.insert(m_jsonKey.id, request.m_id);
			obj.insert(m_jsonKey.detectId, request.m_detectId);
			obj.insert(m_jsonKey.name, request.m_name);
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::DetectPlatformSubtypeCreateRequest & request)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			request.m_id = jsonObject.value(m_jsonKey.id).toInt();
			request.m_detectId = jsonObject.value(m_jsonKey.detectId).toInt();
			request.m_name = jsonObject.value(m_jsonKey.name).toString();
		});
	}

	QByteArray JsonWrapper::wrap(const Datastruct::DetectPlatformSubtypeCreateResponse & response)
	{
		return wrapObject([&](QJsonObject & obj) {
			obj.insert(m_jsonKey.result, response.m_createResult);
			obj.insert(m_jsonKey.errorInfo, response.m_errorInfo);

			if (response.m_createResult) {
				QJsonObject dataObj;
				dataObj.insert(m_jsonKey.id, response.m_dutyRecordInfo.id);
				dataObj.insert(m_jsonKey.detectId, response.m_dutyRecordInfo.detectId);
				dataObj.insert(m_jsonKey.name, response.m_dutyRecordInfo.name);
				obj.insert(m_jsonKey.data, dataObj);
			}
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::DetectPlatformSubtypeCreateResponse & response)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			response.m_createResult = jsonObject.value(m_jsonKey.result).toBool();
			response.m_errorInfo = static_cast<Datastruct::ErrorCode>(jsonObject.value(m_jsonKey.errorInfo).toInt());

			if (response.m_createResult) {
				QJsonObject dataObj = jsonObject.value(m_jsonKey.data).toObject();
				if (dataObj.isEmpty())
					return;
				response.m_dutyRecordInfo.id = jsonObject.value(m_jsonKey.id).toInt();
				response.m_dutyRecordInfo.detectId = jsonObject.value(m_jsonKey.detectId).toInt();
				response.m_dutyRecordInfo.name = jsonObject.value(m_jsonKey.name).toString();
			}
		});
	}

	QByteArray JsonWrapper::wrap(const Datastruct::LoadAllDetectPlatformSubtypesRequest & request)
	{
		return wrapObject([&](QJsonObject & obj) {
			obj.insert(m_jsonKey.detectId, request.m_detectId);
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::LoadAllDetectPlatformSubtypesRequest & request)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			request.m_detectId = jsonObject.value(m_jsonKey.detectId).toInt();
		});
	}

	QByteArray JsonWrapper::wrap(const Datastruct::LoadAllDetectPlatformSubtypesResponse & response)
	{
		return wrapObject([&](QJsonObject & obj) {
			QJsonArray jarray;
			for (int i = 0; i < response.m_detectPlatformSubtypeInfos.size(); i++) {
				const Datastruct::DetectPlatformSubtypeEntityData & dRdata = response.m_detectPlatformSubtypeInfos.at(i);

				QJsonObject dataObj;
				dataObj.insert(m_jsonKey.id, dRdata.id);
				dataObj.insert(m_jsonKey.detectId, dRdata.detectId);
				dataObj.insert(m_jsonKey.name, dRdata.name);
				jarray.append(dataObj);
			}
			obj.insert(m_jsonKey.detectId, response.m_detectId);
			obj.insert(m_jsonKey.totalDataSize, response.m_detectPlatformSubtypeCount);
			obj.insert(m_jsonKey.data, jarray);
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::LoadAllDetectPlatformSubtypesResponse & response)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			QJsonArray jarray = jsonObject.value(m_jsonKey.data).toArray();
			for (int i = 0; i < jarray.size(); i++) {
				Datastruct::DetectPlatformSubtypeEntityData data;
				QJsonObject dataObj = jarray.at(i).toObject();
				data.id = dataObj.value(m_jsonKey.id).toInt();
				data.detectId = dataObj.value(m_jsonKey.detectId).toInt();
				data.name = dataObj.value(m_jsonKey.name).toString();
				response.m_detectPlatformSubtypeInfos.append(data);
			}
			response.m_detectPlatformSubtypeCount = jsonObject.value(m_jsonKey.totalDataSize).toInt();
			response.m_detectId = jsonObject.value(m_jsonKey.detectId).toInt();
		});
	}

	QByteArray JsonWrapper::wrap(const Datastruct::DetectPlatformSubtypeDeleteRequest & request)
	{
		return wrapObject([&](QJsonObject & obj) {
			obj.insert(m_jsonKey.id, request.m_id);
			obj.insert(m_jsonKey.detectId, request.m_detectId);
			obj.insert(m_jsonKey.name, request.m_name);
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::DetectPlatformSubtypeDeleteRequest & request)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			request.m_id = jsonObject.value(m_jsonKey.id).toInt();
			request.m_detectId = jsonObject.value(m_jsonKey.detectId).toInt();
			request.m_name = jsonObject.value(m_jsonKey.name).toString();
		});
	}

	QByteArray JsonWrapper::wrap(const Datastruct::DetectPlatformSubtypeDeleteResponse & response)
	{
		return wrapObject([&](QJsonObject & obj) {
			obj.insert(m_jsonKey.result, response.m_deleteResult);
			obj.insert(m_jsonKey.errorInfo, response.m_errorInfo);
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::DetectPlatformSubtypeDeleteResponse & response)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			response.m_deleteResult = jsonObject.value(m_jsonKey.result).toBool();
			response.m_errorInfo = static_cast<Datastruct::ErrorCode>(jsonObject.value(m_jsonKey.errorInfo).toInt());
		});
	}

	QByteArray JsonWrapper::wrap(const Datastruct::DetectPlatformSubtypeModifyRequest & request)
	{
		return wrapObject([&](QJsonObject & obj) {
			obj.insert(m_jsonKey.id, request.m_id);
			obj.insert(m_jsonKey.detectId, request.m_detectId);
			obj.insert(m_jsonKey.name, request.m_name);
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::DetectPlatformSubtypeModifyRequest & request)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			request.m_id = jsonObject.value(m_jsonKey.id).toInt();
			request.m_detectId = jsonObject.value(m_jsonKey.detectId).toInt();
			request.m_name = jsonObject.value(m_jsonKey.name).toString();
		});
	}

	QByteArray JsonWrapper::wrap(const Datastruct::DetectPlatformSubtypeModifyResponse & response)
	{
		return wrapObject([&](QJsonObject & obj) {
			obj.insert(m_jsonKey.result, response.m_modifyResult);
			obj.insert(m_jsonKey.errorInfo, response.m_errorInfo);
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::DetectPlatformSubtypeModifyResponse & response)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			response.m_modifyResult = jsonObject.value(m_jsonKey.result).toBool();
			response.m_errorInfo = static_cast<Datastruct::ErrorCode>(jsonObject.value(m_jsonKey.errorInfo).toInt());
		});
	}

	QByteArray JsonWrapper::wrap(const Datastruct::TargetCreateRequest & request)
	{
		return wrapObject([&](QJsonObject & obj) {
			obj.insert(m_jsonKey.id, request.m_id);
			obj.insert(m_jsonKey.name, request.m_name);
			obj.insert(m_jsonKey.type, request.m_type);
			obj.insert(m_jsonKey.createTime, request.m_creanTime);
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::TargetCreateRequest & request)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			request.m_id = jsonObject.value(m_jsonKey.id).toString();
			request.m_name = jsonObject.value(m_jsonKey.name).toString();
			request.m_type = jsonObject.value(m_jsonKey.type).toInt();
			request.m_creanTime = jsonObject.value(m_jsonKey.createTime).toString();
		});
	}

	QByteArray JsonWrapper::wrap(const Datastruct::TargetCreateResponse & response)
	{
		return wrapObject([&](QJsonObject & obj) {
			obj.insert(m_jsonKey.result, response.m_createResult);
			obj.insert(m_jsonKey.errorInfo, response.m_errorInfo);

			if (response.m_createResult) {
				QJsonObject dataObj;
				dataObj.insert(m_jsonKey.id, response.m_targetInfo.id);
				dataObj.insert(m_jsonKey.name, response.m_targetInfo.name);
				dataObj.insert(m_jsonKey.type, response.m_targetInfo.type);
				dataObj.insert(m_jsonKey.createTime, response.m_targetInfo.createTime);
				obj.insert(m_jsonKey.data, dataObj);
			}
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::TargetCreateResponse & response)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			response.m_createResult = jsonObject.value(m_jsonKey.result).toBool();
			response.m_errorInfo = static_cast<Datastruct::ErrorCode>(jsonObject.value(m_jsonKey.errorInfo).toInt());

			if (response.m_createResult) {
				QJsonObject dataObj = jsonObject.value(m_jsonKey.data).toObject();
				if (dataObj.isEmpty())
					return;
				response.m_targetInfo.id = jsonObject.value(m_jsonKey.id).toString();
				response.m_targetInfo.name = jsonObject.value(m_jsonKey.name).toString();
				response.m_targetInfo.type = jsonObject.value(m_jsonKey.type).toInt();
				response.m_targetInfo.createTime = jsonObject.value(m_jsonKey.createTime).toString();
			}
		});
	}

	QByteArray JsonWrapper::wrap(const Datastruct::LoadAllTargetRequest & request)
	{
		return wrapObject([&](QJsonObject & obj) {
			obj.insert(m_jsonKey.limitIndex, request.m_limitIndex);
			obj.insert(m_jsonKey.offsetIndex, request.m_offsetIndex);
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::LoadAllTargetRequest & request)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			request.m_limitIndex = jsonObject.value(m_jsonKey.limitIndex).toInt();
			request.m_offsetIndex = jsonObject.value(m_jsonKey.offsetIndex).toInt();
		});
	}

	QByteArray JsonWrapper::wrap(const Datastruct::LoadAllTargetResponse & response)
	{
		return wrapObject([&](QJsonObject & obj) {
			QJsonArray jarray;
			for (int i = 0; i < response.m_targetInfos.size(); i++) {
				const Datastruct::TargetEntityData & dRdata = response.m_targetInfos.at(i);

				QJsonObject dataObj;
				dataObj.insert(m_jsonKey.id, dRdata.id);
				dataObj.insert(m_jsonKey.name, dRdata.name);
				dataObj.insert(m_jsonKey.type, dRdata.type);
				dataObj.insert(m_jsonKey.createTime, dRdata.createTime);
				jarray.append(dataObj);
			}
			obj.insert(m_jsonKey.count, response.m_targetCount);
			obj.insert(m_jsonKey.data, jarray);
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::LoadAllTargetResponse & response)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			QJsonArray jarray = jsonObject.value(m_jsonKey.data).toArray();
			for (int i = 0; i < jarray.size(); i++) {
				Datastruct::TargetEntityData data;
				QJsonObject dataObj = jarray.at(i).toObject();
				
				data.id = dataObj.value(m_jsonKey.id).toString();
				data.name = dataObj.value(m_jsonKey.name).toString();
				data.type = dataObj.value(m_jsonKey.type).toInt();
				data.createTime = dataObj.value(m_jsonKey.createTime).toString();
				response.m_targetInfos.append(data);
			}
			response.m_targetCount = jsonObject.value(m_jsonKey.count).toInt();
		});
	}

	QByteArray JsonWrapper::wrap(const Datastruct::TargetDeleteRequest & request)
	{
		return wrapObject([&](QJsonObject & obj) {
			obj.insert(m_jsonKey.id, request.m_id);
			obj.insert(m_jsonKey.name, request.m_name);
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::TargetDeleteRequest & request)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			request.m_id = jsonObject.value(m_jsonKey.id).toInt();
			request.m_name = jsonObject.value(m_jsonKey.name).toString();
		});
	}

	QByteArray JsonWrapper::wrap(const Datastruct::TargetDeleteResponse & response)
	{
		return wrapObject([&](QJsonObject & obj) {
			obj.insert(m_jsonKey.result, response.m_deleteResult);
			obj.insert(m_jsonKey.errorInfo, response.m_errorInfo);
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::TargetDeleteResponse & response)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			response.m_deleteResult = jsonObject.value(m_jsonKey.result).toBool();
			response.m_errorInfo = static_cast<Datastruct::ErrorCode>(jsonObject.value(m_jsonKey.errorInfo).toInt());
		});
	}

	QByteArray JsonWrapper::wrap(const Datastruct::TargetModifyRequest & request)
	{
		return wrapObject([&](QJsonObject & obj) {
			obj.insert(m_jsonKey.id, request.m_id);
			obj.insert(m_jsonKey.name, request.m_name);
			obj.insert(m_jsonKey.type, request.m_type);
			//obj.insert(m_jsonKey.createTime, request.m_createName);
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::TargetModifyRequest & request)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			request.m_id = jsonObject.value(m_jsonKey.id).toString();
			request.m_name = jsonObject.value(m_jsonKey.name).toString();
			request.m_type = jsonObject.value(m_jsonKey.type).toInt();
			//request.m_createName = jsonObject.value(m_jsonKey.createTime).toString();
		});
	}

	QByteArray JsonWrapper::wrap(const Datastruct::TargetModifyResponse & response)
	{
		return wrapObject([&](QJsonObject & obj) {
			obj.insert(m_jsonKey.result, response.m_modifyResult);
			obj.insert(m_jsonKey.errorInfo, response.m_errorInfo);
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::TargetModifyResponse & response)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			response.m_modifyResult = jsonObject.value(m_jsonKey.result).toBool();
			response.m_errorInfo = static_cast<Datastruct::ErrorCode>(jsonObject.value(m_jsonKey.errorInfo).toInt());
		});
	}

	QByteArray JsonWrapper::wrap(const Datastruct::AISDataCreateRequest & request)
	{
		return wrapObject([&](QJsonObject & obj) {
			obj.insert(m_jsonKey.id, request.m_id);
			obj.insert(m_jsonKey.targetId, request.m_targetId);
			obj.insert(m_jsonKey.mmsi, request.m_mmsi);
			obj.insert(m_jsonKey.time, request.m_time);
			obj.insert(m_jsonKey.lon, request.m_lon);
			obj.insert(m_jsonKey.lat, request.m_lat);
			obj.insert(m_jsonKey.course, request.m_course);
			obj.insert(m_jsonKey.truehead, request.m_truehead);
			obj.insert(m_jsonKey.name, request.m_name);
			obj.insert(m_jsonKey.shipType, request.m_shipType);
			obj.insert(m_jsonKey.shipImo, request.m_shipImo);
			obj.insert(m_jsonKey.navStatus, request.m_navStatus);
			obj.insert(m_jsonKey.speed, request.m_speed);
			obj.insert(m_jsonKey.eta, request.m_eta);
			obj.insert(m_jsonKey.dest, request.m_dest);
			obj.insert(m_jsonKey.length, request.m_length);
			obj.insert(m_jsonKey.width, request.m_width);
			obj.insert(m_jsonKey.callsign, request.m_callsign);
			obj.insert(m_jsonKey.flag, request.m_flag);
			obj.insert(m_jsonKey.buildDate, request.m_buildDate);
			obj.insert(m_jsonKey.port, request.m_port);
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::AISDataCreateRequest & request)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			request.m_id = jsonObject.value(m_jsonKey.id).toString();
			request.m_targetId = jsonObject.value(m_jsonKey.targetId).toString();
			request.m_mmsi = jsonObject.value(m_jsonKey.mmsi).toInt();
			request.m_time = jsonObject.value(m_jsonKey.time).toInt();
			request.m_lon = jsonObject.value(m_jsonKey.lon).toDouble();
			request.m_lat = jsonObject.value(m_jsonKey.lat).toDouble();
			request.m_course = jsonObject.value(m_jsonKey.course).toDouble();
			request.m_truehead = jsonObject.value(m_jsonKey.truehead).toInt();
			request.m_name = jsonObject.value(m_jsonKey.name).toString();
			request.m_shipType = jsonObject.value(m_jsonKey.shipType).toInt();
			request.m_shipImo = jsonObject.value(m_jsonKey.shipImo).toInt();
			request.m_navStatus = jsonObject.value(m_jsonKey.navStatus).toInt();
			request.m_speed = jsonObject.value(m_jsonKey.speed).toDouble();
			request.m_eta = jsonObject.value(m_jsonKey.eta).toString();
			request.m_dest = jsonObject.value(m_jsonKey.dest).toString();
			request.m_length = jsonObject.value(m_jsonKey.length).toDouble();
			request.m_width = jsonObject.value(m_jsonKey.width).toDouble();
			request.m_callsign = jsonObject.value(m_jsonKey.callsign).toString();
			request.m_flag = jsonObject.value(m_jsonKey.flag).toString();
			request.m_buildDate = jsonObject.value(m_jsonKey.buildDate).toString();
			request.m_port = jsonObject.value(m_jsonKey.port).toString();
		});
	}

	QByteArray JsonWrapper::wrap(const Datastruct::AISDataCreateResponse & response)
	{
		return wrapObject([&](QJsonObject & obj) {
			obj.insert(m_jsonKey.result, response.m_createResult);
			obj.insert(m_jsonKey.errorInfo, response.m_errorInfo);

			if (response.m_createResult) {
				QJsonObject dataObj;
				dataObj.insert(m_jsonKey.id, response.m_targetInfo.id);

				obj.insert(m_jsonKey.data, dataObj);
			}
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::AISDataCreateResponse & response)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			response.m_createResult = jsonObject.value(m_jsonKey.result).toBool();
			response.m_errorInfo = static_cast<Datastruct::ErrorCode>(jsonObject.value(m_jsonKey.errorInfo).toInt());

			if (response.m_createResult) {
				QJsonObject dataObj = jsonObject.value(m_jsonKey.data).toObject();
				if (dataObj.isEmpty())
					return;
				response.m_targetInfo.id = jsonObject.value(m_jsonKey.id).toString();
			}
		});
	}

	QByteArray JsonWrapper::wrap(const Datastruct::LoadAllAISDataRequest & request)
	{
		return wrapObject([&](QJsonObject & obj) {
			obj.insert(m_jsonKey.limitIndex, request.m_limitIndex);
			obj.insert(m_jsonKey.offsetIndex, request.m_offsetIndex);
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::LoadAllAISDataRequest & request)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			request.m_limitIndex = jsonObject.value(m_jsonKey.limitIndex).toInt();
			request.m_offsetIndex = jsonObject.value(m_jsonKey.offsetIndex).toInt();
		});
	}

	QByteArray JsonWrapper::wrap(const Datastruct::LoadAllAISDatasResponse & response)
	{
		return wrapObject([&](QJsonObject & obj) {
			QJsonArray jarray;
			for (int i = 0; i < response.m_aisDataInfos.size(); i++) {
				const Datastruct::AisEntityData & dRdata = response.m_aisDataInfos.at(i);

				QJsonObject dataObj;
				dataObj.insert(m_jsonKey.id, dRdata.id);
				dataObj.insert(m_jsonKey.targetId, dRdata.targetId);
				dataObj.insert(m_jsonKey.mmsi, dRdata.mmsi);
				dataObj.insert(m_jsonKey.time, dRdata.time);
				dataObj.insert(m_jsonKey.lon, dRdata.lon);
				dataObj.insert(m_jsonKey.lat, dRdata.lat);
				dataObj.insert(m_jsonKey.course, dRdata.course);
				dataObj.insert(m_jsonKey.truehead, dRdata.truehead);
				dataObj.insert(m_jsonKey.name, dRdata.name);
				dataObj.insert(m_jsonKey.shipType, dRdata.shipType);
				dataObj.insert(m_jsonKey.shipImo, dRdata.shipImo);
				dataObj.insert(m_jsonKey.navStatus, dRdata.navStatus);
				dataObj.insert(m_jsonKey.speed, dRdata.speed);
				dataObj.insert(m_jsonKey.eta, dRdata.eta);
				dataObj.insert(m_jsonKey.dest, dRdata.dest);
				dataObj.insert(m_jsonKey.length, dRdata.length);
				dataObj.insert(m_jsonKey.width, dRdata.width);
				dataObj.insert(m_jsonKey.callsign, dRdata.callsign);
				dataObj.insert(m_jsonKey.flag, dRdata.flag);
				dataObj.insert(m_jsonKey.buildDate, dRdata.buildDate);
				dataObj.insert(m_jsonKey.port, dRdata.port);

				jarray.append(dataObj);
			}
			obj.insert(m_jsonKey.count, response.m_aisDataCount);
			obj.insert(m_jsonKey.data, jarray);
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::LoadAllAISDatasResponse & response)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			QJsonArray jarray = jsonObject.value(m_jsonKey.data).toArray();
			for (int i = 0; i < jarray.size(); i++) {

				Datastruct::AisEntityData data;
				QJsonObject dataObj = jarray.at(i).toObject();

				data.id = dataObj.value(m_jsonKey.id).toString();
				data.targetId = dataObj.value(m_jsonKey.targetId).toString();
				data.mmsi = dataObj.value(m_jsonKey.mmsi).toInt();
				data.time = dataObj.value(m_jsonKey.time).toInt();
				data.lon = dataObj.value(m_jsonKey.lon).toDouble();
				data.lat = dataObj.value(m_jsonKey.lat).toDouble();
				data.course = dataObj.value(m_jsonKey.course).toDouble();
				data.truehead = dataObj.value(m_jsonKey.truehead).toInt();
				data.name = dataObj.value(m_jsonKey.name).toString();
				data.shipType = dataObj.value(m_jsonKey.shipType).toInt();
				data.shipImo = dataObj.value(m_jsonKey.shipImo).toInt();
				data.navStatus = dataObj.value(m_jsonKey.navStatus).toInt();
				data.speed = dataObj.value(m_jsonKey.speed).toDouble();
				data.eta = dataObj.value(m_jsonKey.eta).toString();
				data.dest = dataObj.value(m_jsonKey.dest).toString();
				data.length = dataObj.value(m_jsonKey.length).toDouble();
				data.width = dataObj.value(m_jsonKey.width).toDouble();
				data.callsign = dataObj.value(m_jsonKey.callsign).toString();
				data.flag = dataObj.value(m_jsonKey.flag).toString();
				data.buildDate = dataObj.value(m_jsonKey.buildDate).toString();
				data.port = dataObj.value(m_jsonKey.port).toString();

				response.m_aisDataInfos.append(data);
			}
			response.m_aisDataCount = jsonObject.value(m_jsonKey.count).toInt();
		});
	}

	QByteArray JsonWrapper::wrap(const Datastruct::AISDataByConditionRequest & request)
	{
		return wrapObject([&](QJsonObject & obj) {
			obj.insert(m_jsonKey.id, request.m_id);
			obj.insert(m_jsonKey.targetId, request.m_targetId);
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::AISDataByConditionRequest & request)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			request.m_id = jsonObject.value(m_jsonKey.id).toString();
			request.m_targetId = jsonObject.value(m_jsonKey.targetId).toString();
		});
	}

	QByteArray JsonWrapper::wrap(const Datastruct::AISDataByConditionResponse & response)
	{
		return wrapObject([&](QJsonObject & obj) {
			QJsonArray jarray;
			for (int i = 0; i < response.m_aisDataInfos.size(); i++) {
				const Datastruct::AisEntityData & dRdata = response.m_aisDataInfos.at(i);

				QJsonObject dataObj;
				dataObj.insert(m_jsonKey.id, dRdata.id);
				dataObj.insert(m_jsonKey.targetId, dRdata.targetId);
				dataObj.insert(m_jsonKey.mmsi, dRdata.mmsi);
				dataObj.insert(m_jsonKey.time, dRdata.time);
				dataObj.insert(m_jsonKey.lon, dRdata.lon);
				dataObj.insert(m_jsonKey.lat, dRdata.lat);
				dataObj.insert(m_jsonKey.course, dRdata.course);
				dataObj.insert(m_jsonKey.truehead, dRdata.truehead);
				dataObj.insert(m_jsonKey.name, dRdata.name);
				dataObj.insert(m_jsonKey.shipType, dRdata.shipType);
				dataObj.insert(m_jsonKey.shipImo, dRdata.shipImo);
				dataObj.insert(m_jsonKey.navStatus, dRdata.navStatus);
				dataObj.insert(m_jsonKey.speed, dRdata.speed);
				dataObj.insert(m_jsonKey.eta, dRdata.eta);
				dataObj.insert(m_jsonKey.dest, dRdata.dest);
				dataObj.insert(m_jsonKey.length, dRdata.length);
				dataObj.insert(m_jsonKey.width, dRdata.width);
				dataObj.insert(m_jsonKey.callsign, dRdata.callsign);
				dataObj.insert(m_jsonKey.flag, dRdata.flag);
				dataObj.insert(m_jsonKey.buildDate, dRdata.buildDate);
				dataObj.insert(m_jsonKey.port, dRdata.port);

				jarray.append(dataObj);
			}
			obj.insert(m_jsonKey.count, response.m_aisDataCount);
			obj.insert(m_jsonKey.data, jarray);
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::AISDataByConditionResponse & response)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			QJsonArray jarray = jsonObject.value(m_jsonKey.data).toArray();
			for (int i = 0; i < jarray.size(); i++) {

				Datastruct::AisEntityData data;
				QJsonObject dataObj = jarray.at(i).toObject();

				data.id = dataObj.value(m_jsonKey.id).toString();
				data.targetId = dataObj.value(m_jsonKey.targetId).toString();
				data.mmsi = dataObj.value(m_jsonKey.mmsi).toInt();
				data.time = dataObj.value(m_jsonKey.time).toInt();
				data.lon = dataObj.value(m_jsonKey.lon).toDouble();
				data.lat = dataObj.value(m_jsonKey.lat).toDouble();
				data.course = dataObj.value(m_jsonKey.course).toDouble();
				data.truehead = dataObj.value(m_jsonKey.truehead).toInt();
				data.name = dataObj.value(m_jsonKey.name).toString();
				data.shipType = dataObj.value(m_jsonKey.shipType).toInt();
				data.shipImo = dataObj.value(m_jsonKey.shipImo).toInt();
				data.navStatus = dataObj.value(m_jsonKey.navStatus).toInt();
				data.speed = dataObj.value(m_jsonKey.speed).toDouble();
				data.eta = dataObj.value(m_jsonKey.eta).toString();
				data.dest = dataObj.value(m_jsonKey.dest).toString();
				data.length = dataObj.value(m_jsonKey.length).toDouble();
				data.width = dataObj.value(m_jsonKey.width).toDouble();
				data.callsign = dataObj.value(m_jsonKey.callsign).toString();
				data.flag = dataObj.value(m_jsonKey.flag).toString();
				data.buildDate = dataObj.value(m_jsonKey.buildDate).toString();
				data.port = dataObj.value(m_jsonKey.port).toString();

				response.m_aisDataInfos.append(data);
			}
			response.m_aisDataCount = jsonObject.value(m_jsonKey.count).toInt();
		});
	}

	QByteArray JsonWrapper::wrap(const Datastruct::AISDataDeleteRequest & request)
	{
		return wrapObject([&](QJsonObject & obj) {
			obj.insert(m_jsonKey.id, request.m_id);
			obj.insert(m_jsonKey.targetId, request.m_targetId);
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::AISDataDeleteRequest & request)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			request.m_id = jsonObject.value(m_jsonKey.id).toString();
			request.m_targetId = jsonObject.value(m_jsonKey.targetId).toString();
		});
	}

	QByteArray JsonWrapper::wrap(const Datastruct::AISDataDeleteResponse & response)
	{
		return wrapObject([&](QJsonObject & obj) {
			obj.insert(m_jsonKey.result, response.m_deleteResult);
			obj.insert(m_jsonKey.errorInfo, response.m_errorInfo);
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::AISDataDeleteResponse & response)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			response.m_deleteResult = jsonObject.value(m_jsonKey.result).toBool();
			response.m_errorInfo = static_cast<Datastruct::ErrorCode>(jsonObject.value(m_jsonKey.errorInfo).toInt());
		});
	}

	QByteArray JsonWrapper::wrap(const Datastruct::AISDataModifyRequest & request)
	{
		return wrapObject([&](QJsonObject & obj) {
			obj.insert(m_jsonKey.id,		request.m_id);
			obj.insert(m_jsonKey.targetId,	request.m_targetId);
			obj.insert(m_jsonKey.mmsi,		request.m_mmsi);
			obj.insert(m_jsonKey.time,		request.m_time);
			obj.insert(m_jsonKey.lon,		request.m_lon);
			obj.insert(m_jsonKey.lat,		request.m_lat);
			obj.insert(m_jsonKey.course,	request.m_course);
			obj.insert(m_jsonKey.truehead,	request.m_truehead);
			obj.insert(m_jsonKey.name,		request.m_name);
			obj.insert(m_jsonKey.shipType,	request.m_shipType);
			obj.insert(m_jsonKey.shipImo,	request.m_shipImo);
			obj.insert(m_jsonKey.navStatus, request.m_navStatus);
			obj.insert(m_jsonKey.speed,		request.m_speed);
			obj.insert(m_jsonKey.eta,		request.m_eta);
			obj.insert(m_jsonKey.dest,		request.m_dest);
			obj.insert(m_jsonKey.length,	request.m_length);
			obj.insert(m_jsonKey.width,		request.m_width);
			obj.insert(m_jsonKey.callsign,	request.m_callsign);
			obj.insert(m_jsonKey.flag,		request.m_flag);
			obj.insert(m_jsonKey.buildDate, request.m_buildDate);
			obj.insert(m_jsonKey.port,		request.m_port);
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::AISDataModifyRequest & request)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			request.m_id		= jsonObject.value(m_jsonKey.id).toString();
			request.m_targetId	= jsonObject.value(m_jsonKey.taskId).toString();
			request.m_mmsi		= jsonObject.value(m_jsonKey.mmsi).toInt();
			request.m_time		= jsonObject.value(m_jsonKey.time).toInt();
			request.m_lon		= jsonObject.value(m_jsonKey.lon).toDouble();
			request.m_lat		= jsonObject.value(m_jsonKey.lat).toDouble();
			request.m_course	= jsonObject.value(m_jsonKey.course).toDouble();
			request.m_truehead	= jsonObject.value(m_jsonKey.truehead).toInt();
			request.m_name		= jsonObject.value(m_jsonKey.name).toString();
			request.m_shipType	= jsonObject.value(m_jsonKey.shipType).toInt();
			request.m_shipImo	= jsonObject.value(m_jsonKey.shipImo).toInt();
			request.m_navStatus = jsonObject.value(m_jsonKey.navStatus).toInt();
			request.m_speed		= jsonObject.value(m_jsonKey.speed).toDouble();
			request.m_eta		= jsonObject.value(m_jsonKey.eta).toString();
			request.m_dest		= jsonObject.value(m_jsonKey.dest).toString();
			request.m_length	= jsonObject.value(m_jsonKey.length).toDouble();
			request.m_width		= jsonObject.value(m_jsonKey.width).toDouble();
			request.m_callsign	= jsonObject.value(m_jsonKey.callsign).toString();
			request.m_flag		= jsonObject.value(m_jsonKey.flag).toString();
			request.m_buildDate = jsonObject.value(m_jsonKey.buildDate).toString();
			request.m_port		= jsonObject.value(m_jsonKey.port).toString();
		});
	}

	QByteArray JsonWrapper::wrap(const Datastruct::AISDataModifyResponse & response)
	{
		return wrapObject([&](QJsonObject & obj) {
			obj.insert(m_jsonKey.result,	response.m_modifyResult);
			obj.insert(m_jsonKey.errorInfo, response.m_errorInfo);
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::AISDataModifyResponse & response)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			response.m_modifyResult = jsonObject.value(m_jsonKey.result).toBool();
			response.m_errorInfo	= static_cast<Datastruct::ErrorCode>(jsonObject.value(m_jsonKey.errorInfo).toInt());
		});
	}


} //namespace CommonDefines 