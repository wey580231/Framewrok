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

	QByteArray JsonWrapper::wrap(const Datastruct::TaskCreateRequest & response)
	{
		return wrapObject([&](QJsonObject & obj) {
			obj.insert(m_jsonKey.id, response.taskId);
			obj.insert(m_jsonKey.name, response.taskName);
			obj.insert(m_jsonKey.startTime, response.startTime);
			obj.insert(m_jsonKey.endTime, response.endTime);
			obj.insert(m_jsonKey.location, response.location);
			obj.insert(m_jsonKey.lon, response.lon);
			obj.insert(m_jsonKey.lat, response.lat);
			obj.insert(m_jsonKey.description, response.description);
			obj.insert(m_jsonKey.detectPlatform, response.detectPlatform);
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::TaskCreateRequest & response)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			response.taskId = jsonObject.value(m_jsonKey.id).toString();
			response.taskName = jsonObject.value(m_jsonKey.name).toString();
			response.startTime = jsonObject.value(m_jsonKey.startTime).toString();
			response.endTime = jsonObject.value(m_jsonKey.endTime).toString();
			response.location = jsonObject.value(m_jsonKey.location).toString();
			response.lon = jsonObject.value(m_jsonKey.lon).toDouble();
			response.lat = jsonObject.value(m_jsonKey.lat).toDouble();
			response.description = jsonObject.value(m_jsonKey.description).toString();
			response.detectPlatform = jsonObject.value(m_jsonKey.detectPlatform).toString();
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
				dataObj.insert(m_jsonKey.detectPlatform, response.taskInfo.detectPlatform);
				obj.insert(m_jsonKey.data, dataObj);
			}
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::TaskCreateResponse & response)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			response.m_createResult = jsonObject.value(m_jsonKey.result).toBool();
			response.m_errorInfo = jsonObject.value(m_jsonKey.errorInfo).toString();

			if (response.m_createResult) {
				QJsonObject dataObj = jsonObject.value(m_jsonKey.data).toObject();
				if (dataObj.isEmpty())
					return;

				response.taskInfo.id = dataObj.value(m_jsonKey.id).toString();
				response.taskInfo.taskName = dataObj.value(m_jsonKey.name).toString();
				response.taskInfo.startTime = dataObj.value(m_jsonKey.startTime).toString();
				response.taskInfo.endTime = dataObj.value(m_jsonKey.endTime).toString();
				response.taskInfo.location = dataObj.value(m_jsonKey.location).toString();
				response.taskInfo.lon = dataObj.value(m_jsonKey.lon).toDouble();
				response.taskInfo.lat = dataObj.value(m_jsonKey.lat).toDouble();
				response.taskInfo.description = dataObj.value(m_jsonKey.description).toString();
				response.taskInfo.detectPlatform = dataObj.value(m_jsonKey.detectPlatform).toString();
			}
		});
	}

	QByteArray JsonWrapper::wrap(const Datastruct::LoadAllTaskRequest & request)
	{
		return wrapObject([&](QJsonObject & obj) {
			obj.insert(m_jsonKey.taskId, request.taskId);
			obj.insert(m_jsonKey.offsetIndex, request.m_offsetIndex);
			obj.insert(m_jsonKey.limitIndex, request.m_limitIndex);
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::LoadAllTaskRequest & request)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			request.taskId = jsonObject.value(m_jsonKey.taskId).toString();
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
				dataObj.insert(m_jsonKey.detectPlatform, dRdata.detectPlatform);

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
				data.lon	= dataObj.value(m_jsonKey.lon).toDouble();
				data.lat	= dataObj.value(m_jsonKey.lat).toDouble();
				data.description = dataObj.value(m_jsonKey.description).toString();
				data.detectPlatform = dataObj.value(m_jsonKey.detectPlatform).toString();

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
			response.m_errorInfo = jsonObject.value(m_jsonKey.errorInfo).toString();
		});
	}

	/******************************      值班日志        ******************************************/
	QByteArray JsonWrapper::wrap(const Datastruct::DutyRecordCreateRequest & request)
	{
		return wrapObject([&](QJsonObject & obj) {
			obj.insert(m_jsonKey.id, request.id);
			obj.insert(m_jsonKey.taskId, request.taskId);
			obj.insert(m_jsonKey.createTime, request.createTime);
			obj.insert(m_jsonKey.description, request.description);
			obj.insert(m_jsonKey.seaCondition, request.seaCondition);
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::DutyRecordCreateRequest & request)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			request.id = jsonObject.value(m_jsonKey.id).toString();
			request.taskId = jsonObject.value(m_jsonKey.taskId).toString();
			request.createTime = jsonObject.value(m_jsonKey.createTime).toString();
			request.description = jsonObject.value(m_jsonKey.description).toString();
			request.seaCondition = jsonObject.value(m_jsonKey.seaCondition).toString();
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
				dataObj.insert(m_jsonKey.description, response.dutyRecordInfo.description);
				dataObj.insert(m_jsonKey.seaCondition, response.dutyRecordInfo.seaCondition);
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
				response.dutyRecordInfo.description = dataObj.value(m_jsonKey.description).toString();
				response.dutyRecordInfo.seaCondition = dataObj.value(m_jsonKey.seaCondition).toString();
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
				dataObj.insert(m_jsonKey.description,	dRdata.description);
				dataObj.insert(m_jsonKey.seaCondition,	dRdata.seaCondition);
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
				data.description = dataObj.value(m_jsonKey.description).toString();
				data.seaCondition = dataObj.value(m_jsonKey.seaCondition).toString();
				response.m_dutyRecordInfos.append(data);
			}
			response.m_dutyRecordCount = jsonObject.value(m_jsonKey.totalDataSize).toInt();
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

	QByteArray JsonWrapper::wrap(const Datastruct::DutyRecordDeleteRequest & request)
	{
		return wrapObject([&](QJsonObject & obj) {
			obj.insert(m_jsonKey.id, request.id);
			obj.insert(m_jsonKey.taskId, request.taskId);
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::DutyRecordDeleteRequest & request)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			request.id = jsonObject.value(m_jsonKey.id).toString();
			request.taskId = jsonObject.value(m_jsonKey.taskId).toString();
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
			response.m_errorInfo = jsonObject.value(m_jsonKey.errorInfo).toString();
		});
	}

	/********************************  试验记录  ********************************/
	QByteArray JsonWrapper::wrap(const Datastruct::ExperimentRecordCreateRequest & request)
	{
		return wrapObject([&](QJsonObject & obj) {
			obj.insert(m_jsonKey.id, request.id);
			obj.insert(m_jsonKey.taskId, request.taskId);
			obj.insert(m_jsonKey.platformId, request.platformId);
			obj.insert(m_jsonKey.lon, request.lon);
			obj.insert(m_jsonKey.lat, request.lat);
			obj.insert(m_jsonKey.seaCondition, request.seaCondition);
			obj.insert(m_jsonKey.floatingTime, request.floatingTime);
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::ExperimentRecordCreateRequest & request)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			request.id = jsonObject.value(m_jsonKey.id).toString();
			request.taskId = jsonObject.value(m_jsonKey.taskId).toString();
			request.platformId = jsonObject.value(m_jsonKey.platformId).toString();
			request.lon = jsonObject.value(m_jsonKey.lon).toDouble();
			request.lat = jsonObject.value(m_jsonKey.lat).toDouble();
			request.seaCondition = jsonObject.value(m_jsonKey.seaCondition).toString();
			request.floatingTime = jsonObject.value(m_jsonKey.floatingTime).toString();
		});
	}

	QByteArray JsonWrapper::wrap(const Datastruct::ExperimentRecordCreateResponse & response)
	{
		return wrapObject([&](QJsonObject & obj) {
			obj.insert(m_jsonKey.result, response.m_createResult);
			obj.insert(m_jsonKey.errorInfo, response.m_errorInfo);

			if (response.m_createResult) {
				QJsonObject dataObj;

				dataObj.insert(m_jsonKey.id, response.dutyRecordInfo.id);
				dataObj.insert(m_jsonKey.taskId, response.dutyRecordInfo.taskId);;
				dataObj.insert(m_jsonKey.platformId, response.dutyRecordInfo.platformId);
				dataObj.insert(m_jsonKey.lon, response.dutyRecordInfo.lon);
				dataObj.insert(m_jsonKey.lat, response.dutyRecordInfo.lat);
				dataObj.insert(m_jsonKey.seaCondition, response.dutyRecordInfo.seaCondition);
				dataObj.insert(m_jsonKey.floatingTime, response.dutyRecordInfo.floatingTime);
				obj.insert(m_jsonKey.data, dataObj);
			}
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::ExperimentRecordCreateResponse & response)
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
				response.dutyRecordInfo.platformId = dataObj.value(m_jsonKey.platformId).toString();
				response.dutyRecordInfo.lon = dataObj.value(m_jsonKey.lon).toDouble();
				response.dutyRecordInfo.lat = dataObj.value(m_jsonKey.lat).toDouble();
				response.dutyRecordInfo.seaCondition = dataObj.value(m_jsonKey.seaCondition).toString();
				response.dutyRecordInfo.floatingTime = dataObj.value(m_jsonKey.floatingTime).toString();
			}
		});
	}

	QByteArray JsonWrapper::wrap(const Datastruct::LoadAllExperimentRecordsRequest & request)
	{
		return wrapObject([&](QJsonObject & obj) {
			obj.insert(m_jsonKey.taskId, request.taskId);
			obj.insert(m_jsonKey.offsetIndex, request.m_offsetIndex);
			obj.insert(m_jsonKey.limitIndex, request.m_limitIndex);
		});
	}

	bool JsonWrapper::unrap(const QByteArray & data, Datastruct::LoadAllExperimentRecordsRequest & request)
	{
		return unwrapObject(data, [&](QJsonObject & jsonObject) {
			request.taskId = jsonObject.value(m_jsonKey.taskId).toString();
			request.m_offsetIndex = jsonObject.value(m_jsonKey.offsetIndex).toInt();
			request.m_limitIndex = jsonObject.value(m_jsonKey.limitIndex).toInt();
		});
	}

	QByteArray JsonWrapper::wrap(const Datastruct::LoadAllExperimentRecordsResponse & response)
	{
		return wrapObject([&](QJsonObject & obj) {

			QJsonArray jarray;
			for (int i = 0; i < response.m_listInfos.size(); i++) {
				const Datastruct::ExperimentRecordEntityData & dRdata = response.m_listInfos.at(i);

				QJsonObject dataObj;

				dataObj.insert(m_jsonKey.id, dRdata.id);
				dataObj.insert(m_jsonKey.taskId, dRdata.taskId);
				dataObj.insert(m_jsonKey.platformId, dRdata.platformId);
				dataObj.insert(m_jsonKey.lon, dRdata.lon);
				dataObj.insert(m_jsonKey.lat, dRdata.lat);
				dataObj.insert(m_jsonKey.seaCondition, dRdata.seaCondition);
				dataObj.insert(m_jsonKey.floatingTime, dRdata.floatingTime);
				jarray.append(dataObj);
			}
			obj.insert(m_jsonKey.totalDataSize, response.m_count);
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
				data.lon = dataObj.value(m_jsonKey.lon).toDouble();
				data.lat = dataObj.value(m_jsonKey.lat).toDouble();
				data.seaCondition = dataObj.value(m_jsonKey.seaCondition).toString();
				data.floatingTime = dataObj.value(m_jsonKey.floatingTime).toString();

				response.m_listInfos.append(data);
			}

			response.m_count = jsonObject.value(m_jsonKey.totalDataSize).toInt();
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