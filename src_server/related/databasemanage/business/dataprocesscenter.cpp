#include "dataprocesscenter.h"

#include <QSqlQuery>
#include <QDateTime>
#include <QDebug>

#include <commondefines\sqltable\tablemapper.h>
#include <base\common\sql\rpersistence.h>

#include "../datafiledeal/receivedatafile.h"
#include "../global.h"

namespace Related {

#define TIME_FORMAT "yyyy-MM-dd hh:mm:ss"

	DataProcessCenter::DataProcessCenter(QObject *parent)
		: QObject(parent), m_database(nullptr)
	{
	}

	DataProcessCenter::~DataProcessCenter()
	{
	}

	void DataProcessCenter::bindDatabase(Base::Database * db)
	{
		m_database = db;
	}

	Datastruct::UserLoginResponse DataProcessCenter::processUserLogin(int clientId, const Datastruct::UserLoginRequest & request)
	{
		Datastruct::UserLoginResponse response;

		Table::UserEntity user;

		Base::RSelect rs(user.table);
		rs.select(user.table)
			.createCriteria()
			.add(Base::Restrictions::eq(user.userName, request.m_name));

		QSqlQuery query(m_database->sqlDatabase());

		if (query.exec(rs.sql())) {
			if (query.next()) {
				QString pass = query.value(user.userPassword).toString();
				if (pass != request.m_password) {
					response.m_errorCode = Datastruct::PASSWROD_ERROR;
				}
				else {
					response.m_loginResult = true;

					response.m_userInfo.id = query.value(user.id).toInt();
					response.m_userInfo.name = query.value(user.userName).toString();
					response.m_userInfo.password = query.value(user.userPassword).toString();
					response.m_userInfo.registTime = query.value(user.regitstTime).toDateTime().toString(TIME_FORMAT);
					response.m_userInfo.privilege = query.value(user.privilege).toInt();
					response.m_userInfo.isManager = query.value(user.superManage).toBool();

					Base::RUpdate rud(user.table);
					rud.update(user.table, { {user.lastLoadTime,QDateTime::currentDateTime().toString(TIME_FORMAT)} })
						.createCriteria()
						.add(Base::Restrictions::eq(user.id, response.m_userInfo.id));

					if (!query.exec(rud.sql())) {
						
					}
				}
			}
			else {
				response.m_errorCode = Datastruct::NO_REGIST;
			}
		}
		else {
			response.m_errorCode = Datastruct::SQL_EXECUTE_ERROR;
		}

		return response;
	}

	Datastruct::UserRegistResponse DataProcessCenter::processUserRegist(int clientId, const Datastruct::UserRegistRequest & request)
	{
		Datastruct::UserRegistResponse response;

		Table::UserEntity user;

		Base::RSelect rs(user.table);
		rs.select(user.table)
			.createCriteria()
			.add(Base::Restrictions::eq(user.userName, request.m_name));

		QSqlQuery query(m_database->sqlDatabase());

		do {
			if (query.exec(rs.sql())) {
				if (query.numRowsAffected() > 0) {
					response.m_loginResult = false;
					response.m_errorCode = Datastruct::USER_EXIST;
					break;
				}
			}


			Base::RPersistence rps(user.table);
			rps.insert({
					{user.userName,request.m_name},
					{user.userPassword,request.m_password},
					{user.regitstTime,QDateTime::currentDateTime()},
					{user.privilege,static_cast<int>(Datastruct::ReadOnly)},
					{user.superManage,0}
				});

			if (query.exec(rps.sql())) {
				if (query.numRowsAffected() > 0) {
					response.m_loginResult = true;
				}
			}
			else {
				response.m_errorCode = Datastruct::SQL_EXECUTE_ERROR;
			}

		} while (0);

		return response;
	}

	Datastruct::LoadAllUserResponse DataProcessCenter::processUserList(int clientId, const Datastruct::LoadAllUserRequest & request)
	{
		Datastruct::LoadAllUserResponse response;

		Table::UserEntity user;

		Base::RSelect rs(user.table);
		rs.select(user.table)
			.limit(request.m_offsetIndex, request.m_limitIndex);

		QSqlQuery query(m_database->sqlDatabase());

		if (query.exec(rs.sql())) {
			while (query.next()) {
				Datastruct::UserEntityData data;

				data.id = query.value(user.id).toInt();
				data.name = query.value(user.userName).toString();
				data.registTime = query.value(user.regitstTime).toDateTime().toString(TIME_FORMAT);
				data.lastLoadTime = query.value(user.lastLoadTime).toDateTime().toString(TIME_FORMAT);
				data.privilege = query.value(user.privilege).toInt();
				data.isManager = query.value(user.superManage).toBool();

				response.m_userInfos.append(data);
			}

			Base::RSelect rst(user.table);
			rst.count();

			if (query.exec(rst.sql())) {
				if (query.next()) {
					response.m_userCount = query.value(0).toInt();
				}
			}
		}

		return response;
	}

	Datastruct::OperateUserResponse DataProcessCenter::processUserOperate(int clientId, const Datastruct::OperateUserRequest & request)
	{
		Datastruct::OperateUserResponse response;
		response.m_operateType = request.m_operateType;

		Table::UserEntity user;

		bool hasManagePrivilege = false;
		if (request.m_operateType == Datastruct::EditPrivilege || request.m_operateType == Datastruct::DeleteUser) {
			Base::RSelect rst(user.table);
			rst.select(user.table, { user.superManage })
				.createCriteria()
				.add(Base::Restrictions::eq(user.id, request.m_manageId));

			QSqlQuery query(m_database->sqlDatabase());

			if (query.exec(rst.sql())) {
				if (query.next()) {
					hasManagePrivilege = query.value(user.superManage).toBool();
				}
			}

			if (hasManagePrivilege) {
				if (request.m_operateType == Datastruct::EditPrivilege) {
					Base::RUpdate rud(user.table);
					rud.update(user.table, { {user.privilege,request.m_privilege}
						,{user.superManage,request.m_isManage} })
						.createCriteria()
						.add(Base::Restrictions::eq(user.id, request.m_id));
					if (query.exec(rud.sql())) {
						if (query.numRowsAffected()) {
							response.m_operateResult = true;
						}
						else {
							response.m_errorCode = Datastruct::NO_USER;
						}
					}
				}
				else {
					Base::RDelete rde(user.table);
					rde.createCriteria()
						.add(Base::Restrictions::eq(user.id, request.m_id));

					if (query.exec(rde.sql())) {
						if (query.numRowsAffected()) {
							response.m_operateResult = true;
						}
						else {
							response.m_errorCode = Datastruct::NO_USER;
						}
					}
				}
			}
			else {
				response.m_errorCode = Datastruct::NO_PRIVILEGE;
			}
		}
		else if (request.m_operateType == Datastruct::UpdateInfo) {
			Base::RUpdate rud(user.table);
			rud.update(user.table, { {user.userPassword,request.m_password} })
				.createCriteria()
				.add(Base::Restrictions::eq(user.id, request.m_id));

			QSqlQuery query(m_database->sqlDatabase());
			if (query.exec(rud.sql())) {
				if (query.numRowsAffected()) {
					response.m_operateResult = true;
				}
				else {
					response.m_errorCode = Datastruct::NO_USER;
				}
			}
		}

		return response;
	}

	Datastruct::TaskCreateResponse DataProcessCenter::processTaskCreate(int clientId, const Datastruct::TaskCreateRequest & request)
	{
		Datastruct::TaskCreateResponse response;
		Table::TaskEntity task;
		Base::RPersistence rps(task.table);
		rps.insert({
				{task.id,			request.m_taskId},
				{task.name,			request.m_taskName},
				{task.startTime,	QDateTime::fromString(request.m_startTime, TIME_FORMAT)},
				{task.endTime,		QDateTime::fromString(request.m_endTime, TIME_FORMAT)},
				{task.location,		request.m_location},
				{task.lon,			request.lon},
				{task.lat,			request.lat},
				{task.description,	request.description},
			});

		QSqlQuery query(m_database->sqlDatabase());
		if (query.exec(rps.sql())) {
			if (query.numRowsAffected() > 0) {
				response.m_createResult = true;
			}
		}
		else {
			response.m_errorInfo = Datastruct::SQL_EXECUTE_ERROR;
		}
		return response;
	}

	Datastruct::LoadAllTaskResponse DataProcessCenter::processTaskList(int clientId, const Datastruct::LoadAllTaskRequest & request)
	{
		Datastruct::LoadAllTaskResponse response;
		Table::TaskEntity task;
		Base::RSelect rs(task.table);
		rs.orderBy(task.table, task.startTime, Base::SuperCondition::DESC);
		QSqlQuery query(m_database->sqlDatabase());
		if (query.exec(rs.sql())) {
			while (query.next()) {
				Datastruct::TaskEntityData data;
				data.id = query.value(task.id).toString();
				data.taskName = query.value(task.name).toString();
				data.startTime = query.value(task.startTime).toDateTime().toString(TIME_FORMAT);
				data.endTime = query.value(task.endTime).toDateTime().toString(TIME_FORMAT);
				data.location = query.value(task.location).toString();
				data.lon = query.value(task.lon).toString();
				data.lat = query.value(task.lat).toString();
				data.description = query.value(task.description).toString();
				response.m_taskInfos.append(data);
			}

			Base::RSelect rst(task.table);
			rst.count();
			if (query.exec(rst.sql())) {
				if (query.next()) {
					response.m_count = query.value(0).toInt();
				}
			}
		}
		return response;
	}

	Datastruct::TaskByConditionResponse DataProcessCenter::processTaskByCondition(int clientId, const Datastruct::TaskByConditionRequest & request)
	{
		Datastruct::TaskByConditionResponse response;
		Table::TaskEntity task;
		Base::RSelect rs(task.table);
		if (request.location.isEmpty()) {
			rs.orderBy(task.table, task.startTime, Base::SuperCondition::DESC)
				.createCriteria()
				.add(Base::Restrictions::ge(task.table, task.startTime, request.startTime))
				.add(Base::Restrictions::le(task.table, task.startTime, request.endTime));
		}
		else {
			rs.orderBy(task.table, task.startTime, Base::SuperCondition::DESC)
				.createCriteria()
				.add(Base::Restrictions::eq(task.location, request.location))
				.add(Base::Restrictions::ge(task.table, task.startTime, request.startTime))
				.add(Base::Restrictions::le(task.table, task.startTime, request.endTime));
		}

		QSqlQuery query(m_database->sqlDatabase());

		if (query.exec(rs.sql())) {
			while (query.next()) {
				Datastruct::TaskEntityData data;
				data.id = query.value(task.id).toString();
				data.taskName = query.value(task.name).toString();
				data.startTime = query.value(task.startTime).toDateTime().toString(TIME_FORMAT);
				data.endTime = query.value(task.endTime).toDateTime().toString(TIME_FORMAT);
				data.location = query.value(task.location).toString();
				data.lon = query.value(task.lon).toString();
				data.lat = query.value(task.lat).toString();
				data.description = query.value(task.description).toString();
				response.m_taskInfos.append(data);
			}
			Base::RSelect rst(task.table);
			rst.count();
			if (query.exec(rst.sql())) {
				if (query.next()) {
					response.m_count = query.value(0).toInt();
				}
			}
		}
		return response;
	}

	Datastruct::TaskDeleteResponse DataProcessCenter::processTaskDelete(int clientId, const Datastruct::TaskDeleteRequest & request)
	{
		Datastruct::TaskDeleteResponse response;
		Table::TaskEntity task;
		Base::RDelete rde(task.table);
		rde.createCriteria()
			.add(Base::Restrictions::eq(task.id, request.taskId));
		QSqlQuery query(m_database->sqlDatabase());

		if (query.exec(rde.sql())) {
			if (query.numRowsAffected()) {
				response.m_deleteResult = true;
			}
			else {
				response.m_errorInfo = Datastruct::SQL_EXECUTE_ERROR;
			}
		}
		return response;
	}

	Datastruct::TaskStaticsInfoResponse DataProcessCenter::processTaskStaticsInfo(int clientId, const Datastruct::TaskStaticsInfoRequest & request)
	{
		Datastruct::TaskStaticsInfoResponse response;
		Table::TaskEntity task;
		QSqlQuery query(m_database->sqlDatabase());
		Base::RSelect minRst(task.table);
		// 起始时间
// 		minRst.min(task.startTime);
// 		if (query.exec(minRst.sql())) {
// 			if (query.next()) {
// 				response.allTaskStartTime = query.value(0).toDateTime().toString(TIME_FORMAT);
// 			}
// 		}
// 		// 结束时间
// 		Base::RSelect maxRst(task.table);
// 		maxRst.max(task.endTime);
// 		if (query.exec(maxRst.sql())) {
// 			if (query.next()) {
// 				qDebug() << query.value(0).toDateTime().toString(TIME_FORMAT);
// 				response.allTaskEndTime = query.value(0).toDateTime().toString(TIME_FORMAT);
// 			}
// 		}
		// 海区

		return response;
	}

	Datastruct::TaskSimpleResponse DataProcessCenter::processTaskSimple(int clientId, const Datastruct::TaskSimpleRequest & request)
	{
		Datastruct::TaskSimpleResponse response;
		Table::TaskEntity task;
		Base::RSelect rs(task.table);
		rs.select(task.table)
			.createCriteria()
			.add(Base::Restrictions::eq(task.id, request.taskId));

		QSqlQuery query(m_database->sqlDatabase());
		if (query.exec(rs.sql())) {
			while (query.next()) {
				response.taskInfo.id = query.value(task.id).toString();
				response.taskInfo.taskName = query.value(task.name).toString();
				response.taskInfo.startTime = query.value(task.startTime).toDateTime().toString(TIME_FORMAT);
				response.taskInfo.endTime = query.value(task.endTime).toDateTime().toString(TIME_FORMAT);
				response.taskInfo.location = query.value(task.location).toString();
				response.taskInfo.lon = query.value(task.lon).toString();
				response.taskInfo.lat = query.value(task.lat).toString();
				response.taskInfo.description = query.value(task.description).toString();
			}
			response.m_result = true;
		}
		else
		{
			response.m_errorInfo = Datastruct::SQL_EXECUTE_ERROR;
		}
		return response;
	}

	Datastruct::TaskModifyResponse DataProcessCenter::processTaskModify(int clientId, const Datastruct::TaskModifyRequest & request)
	{
		Datastruct::TaskModifyResponse response;
		Table::TaskEntity task;
		Base::RSelect rs(task.table);
		rs.select(task.table)
			.createCriteria()
			.add(Base::Restrictions::eq(task.id, request.taskId));

		QSqlQuery query(m_database->sqlDatabase());
		if (query.exec(rs.sql())) {
			if (query.numRowsAffected() < 0) {
				response.m_errorInfo = Datastruct::NO_FINDDATA;
			}
			else
			{
				Base::RUpdate rud(task.table);
				rud.update(task.table, {
					{ task.name,		request.taskName },
					{ task.startTime,	QDateTime::fromString(request.startTime, TIME_FORMAT) },
					{ task.endTime,		QDateTime::fromString(request.endTime, TIME_FORMAT) },
					{ task.location,	request.location },
					{ task.lon,			request.lon },
					{ task.lat,			request.lat },
					{ task.description, request.description },
					})
					.createCriteria()
					.add(Base::Restrictions::eq(task.id, request.taskId));

				QSqlQuery query(m_database->sqlDatabase());

				if (query.exec(rud.sql())) {
					if (query.numRowsAffected()) {
						response.m_result = true;
					}
					else {
						response.m_errorInfo = Datastruct::SQL_EXECUTE_ERROR;
					}
				}
			}
		}
		else
		{
			response.m_errorInfo = Datastruct::SQL_EXECUTE_ERROR;
		}
		return response;
	}

	/**************************** 任务侦测平台 *******************************************/
	Datastruct::TaskDetectPlatformCreateResponse DataProcessCenter::processTaskDetectPlatformCreate(int clientId, const Datastruct::TaskDetectPlatformCreateRequest & request)
	{
		Datastruct::TaskDetectPlatformCreateResponse response;
		Table::TaskDetectPlatformEntity taskDetectPlatform;
		Base::RPersistence rps(taskDetectPlatform.table);
		rps.insert({
				{taskDetectPlatform.id,			request.m_id},
				{taskDetectPlatform.taskId,		request.m_taskId},
				{taskDetectPlatform.detectId,	request.m_detectId},
				{taskDetectPlatform.name,		request.m_name},
				{taskDetectPlatform.sensorType,	request.m_sensorType},
				{taskDetectPlatform.platformPower,	request.m_platformPower},
				{taskDetectPlatform.startTime,	QDateTime::fromString(request.m_startTime, TIME_FORMAT)},
				{taskDetectPlatform.endTime,	QDateTime::fromString(request.m_endTime, TIME_FORMAT)},
				{taskDetectPlatform.lastTime,	request.m_lastTime},
			});
		QSqlQuery query(m_database->sqlDatabase());
		if (query.exec(rps.sql())) {
			if (query.numRowsAffected() > 0) {
				response.m_createResult = true;
			}
		}
		else {
			response.m_errorInfo = Datastruct::SQL_EXECUTE_ERROR;
		}
		return response;
	}

	Datastruct::LoadAllTaskDetectPlatformResponse DataProcessCenter::processTaskDetectPlatformList(int clientId, const Datastruct::LoadAllTaskDetectPlatformRequest & request)
	{
		Datastruct::LoadAllTaskDetectPlatformResponse response;
		Table::TaskDetectPlatformEntity taskDetectPlatform;
		Base::RSelect rs(taskDetectPlatform.table);
		rs.select(taskDetectPlatform.table)
			.createCriteria()
			.add(Base::Restrictions::eq(taskDetectPlatform.taskId, request.m_taskId));
		QSqlQuery query(m_database->sqlDatabase());
		if (query.exec(rs.sql())) {
			while (query.next()) {
				Datastruct::TaskDetectPlatformEntityData data;
				data.id			= query.value(taskDetectPlatform.id).toString();
				data.taskId		= query.value(taskDetectPlatform.taskId).toString();
				data.detectId	= query.value(taskDetectPlatform.detectId).toInt();
				data.name		= query.value(taskDetectPlatform.name).toString();
				data.sensorType = query.value(taskDetectPlatform.sensorType).toString();
				data.platformPower = query.value(taskDetectPlatform.platformPower).toInt();
				data.startTime	= query.value(taskDetectPlatform.startTime).toDateTime().toString(TIME_FORMAT);
				data.endTime	= query.value(taskDetectPlatform.endTime).toDateTime().toString(TIME_FORMAT);
				data.lastTime	= query.value(taskDetectPlatform.lastTime).toInt();
				response.m_dataInfos.append(data);
			}
			Base::RSelect rst(taskDetectPlatform.table);
			rst.select(taskDetectPlatform.table).
				createCriteria().
				add(Base::Restrictions::eq(taskDetectPlatform.taskId, request.m_taskId));
			rst.count();
			if (query.exec(rst.sql())) {
				if (query.next()) {
					response.m_taskDetectPlatformCount = query.value(0).toInt();
				}
			}
		}
		return response;
	}

	Datastruct::TaskDetectPlatformByConditionResponse DataProcessCenter::processTaskDetectPlatformByCondition(int clientId, const Datastruct::TaskDetectPlatformByConditionRequest & request)
	{
		return Datastruct::TaskDetectPlatformByConditionResponse();
	}

	Datastruct::TaskDetectPlatformDeleteResponse DataProcessCenter::processTaskDetectPlatformDelete(int clientId, const Datastruct::TaskDetectPlatformDeleteRequest & request)
	{
		Datastruct::TaskDetectPlatformDeleteResponse response;
		Table::TaskDetectPlatformEntity taskDetectPlatform;
		Base::RDelete rde(taskDetectPlatform.table);
		if (!request.m_id.isEmpty()) {
			rde.createCriteria()
				.add(Base::Restrictions::eq(taskDetectPlatform.id, request.m_id));
		}
		if (!request.m_taskId.isEmpty()) {
			rde.createCriteria()
				.add(Base::Restrictions::eq(taskDetectPlatform.taskId, request.m_taskId));
		}
		QSqlQuery query(m_database->sqlDatabase());
		if (query.exec(rde.sql())) {
			if (query.numRowsAffected()) {
				response.m_deleteResult = true;
			}
			else {
				response.m_errorInfo = Datastruct::SQL_EXECUTE_ERROR;
			}
		}
		return response;
	}

	Datastruct::TaskDetectPlatformModifyResponse DataProcessCenter::processTaskDetectPlatformModify(int clientId, const Datastruct::TaskDetectPlatformModifyRequest & request)
	{
		Datastruct::TaskDetectPlatformModifyResponse response;
		Table::TaskDetectPlatformEntity taskDetectPlatform;
		Base::RSelect rs(taskDetectPlatform.table);
		rs.select(taskDetectPlatform.table)
			.createCriteria()
			.add(Base::Restrictions::eq(taskDetectPlatform.id, request.m_id));

		QSqlQuery query(m_database->sqlDatabase());
		if (query.exec(rs.sql())) {
			if (query.numRowsAffected() < 0) {
				response.m_errorInfo = Datastruct::NO_FINDDATA;
			}
			else {
				Base::RUpdate rud(taskDetectPlatform.table);
				rud.update(taskDetectPlatform.table, {

					{taskDetectPlatform.detectId,	request.m_detectId},
					{taskDetectPlatform.name,		request.m_name},
					{taskDetectPlatform.sensorType,	request.m_sensorType},
					{taskDetectPlatform.platformPower,	request.m_platformPower},
					{taskDetectPlatform.startTime,	QDateTime::fromString(request.m_startTime, TIME_FORMAT)},
					{taskDetectPlatform.endTime,	QDateTime::fromString(request.m_endTime, TIME_FORMAT)},
					{taskDetectPlatform.lastTime,	request.m_lastTime},
					})
					.createCriteria()
					.add(Base::Restrictions::eq(taskDetectPlatform.id, request.m_id));
				QSqlQuery query(m_database->sqlDatabase());
				if (query.exec(rud.sql())) {
					if (query.numRowsAffected()) {
						response.m_modifyResult = true;
					}
					else {
						response.m_errorInfo = Datastruct::SQL_EXECUTE_ERROR;
					}
				}
			}
		}
		else {
			response.m_errorInfo = Datastruct::SQL_EXECUTE_ERROR;
		}
		return response;
	}


	/************************ 试验图片资源 *******************************************************/

	Datastruct::TaskDataFileCreateResponse DataProcessCenter::processTaskImageCreate(int clientId, const Datastruct::TaskDataFileCreateRequest & request)
	{
		Datastruct::TaskDataFileCreateResponse response;
		Table::TaskImageEntity taskImage;

		QString fileId = Base::RUtil::UUID();

		//[] 判断文件是否存在
		Base::RSelect rs(taskImage.table);
				rs.select(taskImage.table)
 			.createCriteria()
			.add(Base::Restrictions::eq(taskImage.md5, request.m_md5))
			.add(Base::Restrictions::eq(taskImage.taskId, request.m_taskId));
		QSqlQuery query(m_database->sqlDatabase());

		if (query.exec(rs.sql())) {
			if (query.numRowsAffected() == 0) {

				//[] 插入新信息
				Base::RPersistence rps(taskImage.table);
				rps.insert({
					{taskImage.id,			fileId},
					{taskImage.taskId,		request.m_taskId},
					{taskImage.realName,	request.m_name},
					{taskImage.suffix,		request.m_suffix},
					{taskImage.uploadTime,	QDateTime::fromString(request.m_timeStamp, TIME_FORMAT)},
					{taskImage.imageSize,	(int)request.m_size},
					{taskImage.md5,			request.m_md5},
					{taskImage.savePath,	QString("")},
					{taskImage.revcSize,	0},
					{taskImage.description,	QString("")},
					});
				QSqlQuery query(m_database->sqlDatabase());

				if (query.exec(rps.sql())) {
					if (query.numRowsAffected() > 0) {

						response.m_result = true;
						response.m_taskId = request.m_taskId;
						response.m_md5 = request.m_md5;
						response.m_id = fileId;

						// 接收数据文件
						ReceiveDataFile * t_writeRawDataFile = new ReceiveDataFile();
						t_writeRawDataFile->bindDataProcess(this);
						t_writeRawDataFile->setTaskId(request.m_taskId);
						t_writeRawDataFile->setFileType(Datastruct::File_Image);
						t_writeRawDataFile->setFileName(G_FileSaveRootPath, request.m_name, request.m_suffix);
						G_MapWriteRawDataFile.insert(fileId, t_writeRawDataFile);
					}
				}
				else {
					response.m_errorInfo = Datastruct::SQL_EXECUTE_ERROR;
				}
			}
			else
			{
				Datastruct::TaskImageEntityData data;
				while (query.next()) {
					
					data.taskId		= query.value(taskImage.taskId).toString();
					data.md5		= query.value(taskImage.md5).toString();
					data.id			= query.value(taskImage.id).toString();
					data.imageSize	= query.value(taskImage.imageSize).toLongLong();
					data.recvSize	= query.value(taskImage.revcSize).toLongLong();
				}
				if (data.recvSize != data.imageSize) {
					
					response.m_result = true;
					response.m_taskId = data.taskId;
					response.m_md5	  = data.md5;
					response.m_id	  = data.id;
					response.m_lastLen = data.recvSize;

					// 接收数据文件
					ReceiveDataFile * t_writeRawDataFile = new ReceiveDataFile();
					t_writeRawDataFile->bindDataProcess(this);
					t_writeRawDataFile->setTaskId(request.m_taskId);
					t_writeRawDataFile->setFileType(Datastruct::File_Image);
					t_writeRawDataFile->setFileName(G_FileSaveRootPath, request.m_name, request.m_suffix);
					G_MapWriteRawDataFile.insert(data.id, t_writeRawDataFile);
				}
				else
				{
					response.m_errorInfo = Datastruct::SQL_EXECUTE_ERROR;
				}
			}
		}
		else
		{
			response.m_errorInfo = Datastruct::SQL_EXECUTE_ERROR;
		}

		return response;
	}

	Datastruct::TaskDataFileCreateResponse DataProcessCenter::processTaskOriginalXMLCreate(int clientId, const Datastruct::TaskDataFileCreateRequest & request)
	{
		Datastruct::TaskDataFileCreateResponse response;
		Table::TaskPlatformOriginalXmlEntity xmlEntity;

		QString fileId = Base::RUtil::UUID();
		//[] 判断文件是否存在
		Base::RSelect rs(xmlEntity.table);
		rs.select(xmlEntity.table)
			.createCriteria()
			.add(Base::Restrictions::eq(xmlEntity.md5, request.m_md5))
			.add(Base::Restrictions::eq(xmlEntity.taskId, request.m_taskId));
		QSqlQuery query(m_database->sqlDatabase());

		if (query.exec(rs.sql())) {
			if (query.numRowsAffected() == 0) {
				Base::RPersistence rps(xmlEntity.table);
				rps.insert({
						{xmlEntity.id,				fileId},
						{xmlEntity.taskId,			request.m_taskId},
						{xmlEntity.detectPlatformId,request.m_detectId},
						{xmlEntity.realName,		request.m_name},
						{xmlEntity.fileSize,		request.m_size},
						{xmlEntity.md5,				request.m_md5},
						{xmlEntity.startTime,		QDateTime::fromString(request.m_timeStamp, TIME_FORMAT)},
						{xmlEntity.endTime,			QDateTime::fromString(request.m_timeStamp, TIME_FORMAT)},
						{xmlEntity.savePath,		QString("")},
						{xmlEntity.revcSize,		0},
					});

				QSqlQuery query(m_database->sqlDatabase());

				if (query.exec(rps.sql())) {
					if (query.numRowsAffected() > 0) {
						response.m_result = true;
						response.m_taskId = request.m_taskId;
						response.m_md5 = request.m_md5;
						response.m_id = fileId;

						/*!< 数据接收 */
						ReceiveDataFile * t_writeRawDataFile = new ReceiveDataFile();
						t_writeRawDataFile->bindDataProcess(this);
						t_writeRawDataFile->setTaskId(request.m_taskId);
						t_writeRawDataFile->setFileType(Datastruct::File_XML);
						t_writeRawDataFile->setFileName(G_FileSaveRootPath, request.m_name, request.m_suffix);
						G_MapWriteRawDataFile.insert(fileId, t_writeRawDataFile);
					}
				}
				else {
					response.m_errorInfo = Datastruct::SQL_EXECUTE_ERROR;
				}
			}
			else
			{
				Datastruct::TaskPlatformOriginalXmlEntityData data;
				while (query.next()) {
					data.id = query.value(xmlEntity.id).toString();
					data.taskId = query.value(xmlEntity.taskId).toString();
					data.md5 = query.value(xmlEntity.md5).toString();
					data.fileSize = query.value(xmlEntity.fileSize).toDouble();
					data.recvSize = query.value(xmlEntity.revcSize).toInt();
				}

				if (data.fileSize != data.recvSize) {

					response.m_result = true;
					response.m_taskId = data.taskId;
					response.m_md5	= data.md5;
					response.m_id	= data.id;
					response.m_lastLen = data.recvSize;

					// 接收数据文件
					ReceiveDataFile * t_writeRawDataFile = new ReceiveDataFile();
					t_writeRawDataFile->bindDataProcess(this);
					t_writeRawDataFile->setTaskId(request.m_taskId);
					t_writeRawDataFile->setFileType(Datastruct::File_XML);
					t_writeRawDataFile->setFileName(G_FileSaveRootPath, request.m_name, request.m_suffix);
					G_MapWriteRawDataFile.insert(data.id, t_writeRawDataFile);
				}
				else
				{
					response.m_errorInfo = Datastruct::SQL_EXECUTE_ERROR;
				}
			}
		}
		return response;
	}

	Datastruct::TaskDataFileCreateResponse DataProcessCenter::processTaskOriginalDataCreate(int clientId, const Datastruct::TaskDataFileCreateRequest & request)
	{
		Datastruct::TaskDataFileCreateResponse response;
		Table::TaskplatformOriginalDataEntity dataEntity;
		QString fileId = Base::RUtil::UUID();

		//[] 判断文件是否存在
		Base::RSelect rs(dataEntity.table);
		rs.select(dataEntity.table)
			.createCriteria()
			.add(Base::Restrictions::eq(dataEntity.md5, request.m_md5))
			.add(Base::Restrictions::eq(dataEntity.taskId, request.m_taskId));
		QSqlQuery query(m_database->sqlDatabase());

		if (query.exec(rs.sql())) {
			if (query.numRowsAffected() == 0) {
				Base::RPersistence rps(dataEntity.table);
				rps.insert({
						{dataEntity.id,				fileId},
						{dataEntity.taskId,			request.m_taskId},
						{dataEntity.platformId,		request.m_detectId},
						{dataEntity.realName,		request.m_name},
						{dataEntity.fileSize,		request.m_size},
						{dataEntity.startFrameIndex,0},
						{dataEntity.endFrameIndex,	0},
						{dataEntity.startTime,		QDateTime::fromString(request.m_timeStamp, TIME_FORMAT)},
						{dataEntity.endTime,		QDateTime::fromString(request.m_timeStamp, TIME_FORMAT)},
						{dataEntity.md5,			request.m_md5},
						{dataEntity.revcSize,		0},
						{dataEntity.savePath,		QString("")},
					});

				QSqlQuery query(m_database->sqlDatabase());

				if (query.exec(rps.sql())) {
					if (query.numRowsAffected() > 0) {
						response.m_result = true;
						response.m_taskId = request.m_taskId;
						response.m_md5 = request.m_md5;
						response.m_id = fileId;

						/*!< 数据接收 */
						ReceiveDataFile * t_writeRawDataFile = new ReceiveDataFile();
						t_writeRawDataFile->bindDataProcess(this);
						t_writeRawDataFile->setTaskId(request.m_taskId);
						t_writeRawDataFile->setFileType(Datastruct::File_Dat);
						t_writeRawDataFile->setFileName(G_FileSaveRootPath, request.m_name, request.m_suffix);
						G_MapWriteRawDataFile.insert(fileId, t_writeRawDataFile);
					}
				}
				else {
					response.m_errorInfo = Datastruct::SQL_EXECUTE_ERROR;
				}
			}
			else
			{
				Datastruct::TaskPlatformOriginalDataEntityData data;
				while (query.next()) {
					data.id = query.value(dataEntity.id).toString();
					data.taskId = query.value(dataEntity.taskId).toString();
					data.md5	= query.value(dataEntity.md5).toString();
					data.fileSize = query.value(dataEntity.fileSize).toDouble();
					data.recvSize = query.value(dataEntity.revcSize).toInt();
				}
				if (data.fileSize != data.recvSize) {
					response.m_result = true;
					response.m_taskId = data.taskId;
					response.m_md5 = data.md5;
					response.m_id = data.id;
					response.m_lastLen = data.recvSize;

					// 接收数据文件
					ReceiveDataFile * t_writeRawDataFile = new ReceiveDataFile();
					t_writeRawDataFile->bindDataProcess(this);
					t_writeRawDataFile->setTaskId(request.m_taskId);
					t_writeRawDataFile->setFileType(Datastruct::File_Dat);
					t_writeRawDataFile->setFileName(G_FileSaveRootPath, request.m_name, request.m_suffix);
					G_MapWriteRawDataFile.insert(data.id, t_writeRawDataFile);
				}
				else
				{
					response.m_errorInfo = Datastruct::SQL_EXECUTE_ERROR;
				}
			}
		}
		else
		{
			response.m_errorInfo = Datastruct::SQL_EXECUTE_ERROR;
		}

		return response;
	}

	Datastruct::TaskDataFileDeleteResponse DataProcessCenter::processTaskImageDelete(int clientId, const Datastruct::TaskDataFileDeleteRequest & request)
	{
		Datastruct::TaskDataFileDeleteResponse response;
		Table::TaskImageEntity taskImage;
		Base::RDelete rde(taskImage.table);
		if (!request.m_id.isEmpty()) {
			rde.createCriteria()
				.add(Base::Restrictions::eq(taskImage.id, request.m_id));
		}
		if (!request.m_taskId.isEmpty()) {
			rde.createCriteria()
				.add(Base::Restrictions::eq(taskImage.taskId, request.m_taskId));
		}

		QSqlQuery query(m_database->sqlDatabase());
		if (query.exec(rde.sql())) {
			if (query.numRowsAffected()) {
				response.m_deleteResult = true;
			}
			else {
				response.m_errorInfo = Datastruct::SQL_EXECUTE_ERROR;
			}
		}
		return response;
	}

	Datastruct::TaskDataFileDeleteResponse DataProcessCenter::processTaskOriginalXMLDelete(int clientId, const Datastruct::TaskDataFileDeleteRequest & request)
	{
		Datastruct::TaskDataFileDeleteResponse response;
		Table::TaskPlatformOriginalXmlEntity taskXml;
		Base::RDelete rde(taskXml.table);
		if (!request.m_id.isEmpty()) {
			rde.createCriteria()
				.add(Base::Restrictions::eq(taskXml.id, request.m_id));
		}
		if (!request.m_taskId.isEmpty()) {
			rde.createCriteria()
				.add(Base::Restrictions::eq(taskXml.taskId, request.m_taskId));
		}

		QSqlQuery query(m_database->sqlDatabase());
		if (query.exec(rde.sql())) {
			if (query.numRowsAffected()) {
				response.m_deleteResult = true;
			}
			else {
				response.m_errorInfo = Datastruct::SQL_EXECUTE_ERROR;
			}
		}
		return response;
	}

	Datastruct::TaskDataFileDeleteResponse DataProcessCenter::processTaskOriginalDataDelete(int clientId, const Datastruct::TaskDataFileDeleteRequest & request)
	{
		Datastruct::TaskDataFileDeleteResponse response;
		Table::TaskplatformOriginalDataEntity dataEntity;
		Base::RDelete rde(dataEntity.table);
		if (!request.m_id.isEmpty()) {
			rde.createCriteria()
				.add(Base::Restrictions::eq(dataEntity.id, request.m_id));
		}
		if (!request.m_taskId.isEmpty()) {
			rde.createCriteria()
				.add(Base::Restrictions::eq(dataEntity.taskId, request.m_taskId));
		}

		QSqlQuery query(m_database->sqlDatabase());
		if (query.exec(rde.sql())) {
			if (query.numRowsAffected()) {
				response.m_deleteResult = true;
			}
			else {
				response.m_errorInfo = Datastruct::SQL_EXECUTE_ERROR;
			}
		}
		return response;
	}

	Datastruct::LoadAllTaskImageResponse DataProcessCenter::processTaskImageList(int clientId, const Datastruct::LoadAllTaskImageRequest & request)
	{
		Datastruct::LoadAllTaskImageResponse response;
		Table::TaskImageEntity taskImage;
		Base::RSelect rs(taskImage.table);
		rs.select(taskImage.table)
			.createCriteria()
			.add(Base::Restrictions::eq(taskImage.taskId, request.m_taskId));
		QSqlQuery query(m_database->sqlDatabase());
		if (query.exec(rs.sql())) {
			while (query.next()) {
				Datastruct::TaskImageEntityData data;
				data.id			= query.value(taskImage.id).toString();
				data.taskId		= query.value(taskImage.taskId).toString();
				data.realName   = query.value(taskImage.realName).toString();
				data.suffix		= query.value(taskImage.suffix).toString();
				data.uploadTime = query.value(taskImage.uploadTime).toDateTime().toString(TIME_FORMAT);
				data.imageSize  = query.value(taskImage.imageSize).toDouble();
				data.description = query.value(taskImage.description).toString();
				response.m_taskImageInfos.append(data);
			}

			Base::RSelect rst(taskImage.table);
			rst.select(taskImage.table).
				createCriteria().
				add(Base::Restrictions::eq(taskImage.taskId, request.m_taskId));
			rst.count();

			if (query.exec(rst.sql())) {
				if (query.next()) {
					response.m_taskImageCount = query.value(0).toInt();
				}
			}
		}
		return response;
	}


	Datastruct::DutyRecordCreateResponse DataProcessCenter::processDutyRecordCreate(int clientId, const Datastruct::DutyRecordCreateRequest & request)
	{
		Datastruct::DutyRecordCreateResponse response;

		Table::DutyRecordEntity dutyRecord;

		Base::RPersistence rps(dutyRecord.table);
		rps.insert({
				{dutyRecord.id,				request.m_id},
				{dutyRecord.taskId,			request.m_taskId},
				{dutyRecord.createTime,		QDateTime::fromString(request.m_createTime, TIME_FORMAT)},
				{dutyRecord.description,	request.m_description},
				{dutyRecord.seaCondition,	request.m_seaCondition},
				{dutyRecord.wind,			request.m_wind},
				{dutyRecord.windSpeed,		request.m_windSpeed},
				{dutyRecord.waveHigh,		request.m_waveHigh},
				{dutyRecord.oceanCurrents,	request.m_oceanCurrents},
			});

		QSqlQuery query(m_database->sqlDatabase());

		if (query.exec(rps.sql())) {
			if (query.numRowsAffected() > 0) {
				response.m_createResult = true;
			}
		}
		else {
			response.m_errorInfo = Datastruct::SQL_EXECUTE_ERROR;
		}

		return response;
	}

	Datastruct::LoadAllDutyRecordResponse DataProcessCenter::processDutyRecordList(int clientId, const Datastruct::LoadAllDutyRecordRequest & request)
	{
		Datastruct::LoadAllDutyRecordResponse response;
		Table::DutyRecordEntity dutyRecord;
		Base::RSelect rs(dutyRecord.table);
		rs.orderBy(dutyRecord.table, dutyRecord.createTime, Base::SuperCondition::DESC)
			.createCriteria()
			.add(Base::Restrictions::eq(dutyRecord.taskId, request.m_taskId));

		rs.limit(request.m_offsetIndex, request.m_limitIndex);

		QSqlQuery query(m_database->sqlDatabase());
		if (query.exec(rs.sql())) {
			while (query.next()) {
				Datastruct::DutyRecordEntityData data;
				data.id = query.value(dutyRecord.id).toString();
				data.taskId = query.value(dutyRecord.taskId).toString();
				data.createTime = query.value(dutyRecord.createTime).toDateTime().toString(TIME_FORMAT);
				data.description = query.value(dutyRecord.description).toString();
				data.seaCondition = query.value(dutyRecord.seaCondition).toString();
				data.wind = query.value(dutyRecord.wind).toDouble();
				data.windSpeed = query.value(dutyRecord.windSpeed).toDouble();
				data.waveHigh = query.value(dutyRecord.waveHigh).toDouble();
				data.oceanCurrents = query.value(dutyRecord.oceanCurrents).toDouble();
				response.m_dutyRecordInfos.append(data);
			}

			Base::RSelect rst(dutyRecord.table);
			rst.select(dutyRecord.table).
				createCriteria().
				add(Base::Restrictions::eq(dutyRecord.taskId, request.m_taskId));
			rst.count();
			if (query.exec(rst.sql())) {
				if (query.next()) {
					response.m_dutyRecordCount = query.value(0).toInt();
				}
			}
		}
		return response;
	}
	
	Datastruct::DutyRecordDeleteResponse DataProcessCenter::processDutyRecordDelete(int clientId, const Datastruct::DutyRecordDeleteRequest & request)
	{
		Datastruct::DutyRecordDeleteResponse response;
		Table::DutyRecordEntity dutyRecord;
		Base::RDelete rde(dutyRecord.table);
		rde.createCriteria()
			.add(Base::Restrictions::eq(dutyRecord.id, request.m_id));

		QSqlQuery query(m_database->sqlDatabase());

		if (query.exec(rde.sql())) {
			response.m_deleteResult = true;
		} else {
			response.m_errorInfo = Datastruct::SQL_EXECUTE_ERROR;
		};

		return response;
	}

	Datastruct::DutyRecordModifyResponse DataProcessCenter::processDutyRecordModify(int clientId, const Datastruct::DutyRecordModifyRequest & request)
	{
		Datastruct::DutyRecordModifyResponse response;

		Table::DutyRecordEntity dutyRecord;

		Base::RSelect rs(dutyRecord.table);
		rs.select(dutyRecord.table)
			.createCriteria()
			.add(Base::Restrictions::eq(dutyRecord.id, request.m_id));

		QSqlQuery query(m_database->sqlDatabase());

		if (query.exec(rs.sql())) {
			if (query.numRowsAffected() <  0) {
				response.m_errorInfo = Datastruct::NO_FINDDATA;
			}
			else
			{
				Base::RUpdate rud(dutyRecord.table);
				rud.update(dutyRecord.table, {
					{dutyRecord.description,	request.m_description},
					{dutyRecord.seaCondition,	request.m_seaCondition},
					{dutyRecord.wind,			request.m_wind},
					{dutyRecord.windSpeed,		request.m_windSpeed},
					{dutyRecord.waveHigh,		request.m_waveHigh},
					{dutyRecord.oceanCurrents,	request.m_oceanCurrents},
					})
					.createCriteria()
					.add(Base::Restrictions::eq(dutyRecord.id, request.m_id));
				
				QSqlQuery query(m_database->sqlDatabase());

				if (query.exec(rud.sql())) {
					if (query.numRowsAffected()) {
						response.m_modifyResult = true;
					}
					else {
						response.m_errorInfo = Datastruct::SQL_EXECUTE_ERROR;
					}
				}
			}
		}
		else
		{
			response.m_errorInfo = Datastruct::SQL_EXECUTE_ERROR;
		}

		return response;
	}
	
	Datastruct::ExperimentRecordCreateResponse DataProcessCenter::processExperimentRecordCreate(int clientId, const Datastruct::ExperimentRecordCreateRequest & request)
	{
		Datastruct::ExperimentRecordCreateResponse response;

		Table::ExperimentRecordEntity experimentRecord;

		Base::RPersistence rps(experimentRecord.table);
		rps.insert({
				{experimentRecord.id,					request.m_id},
				{experimentRecord.taskId,				request.m_taskId},
				{experimentRecord.platformId,			request.m_platformId},
				{experimentRecord.floatingTime,			QDateTime::fromString(request.m_floatingTime,TIME_FORMAT)},
				{experimentRecord.lon,					request.m_lon},
				{experimentRecord.lat,					request.m_lat},
				{experimentRecord.setHeadingDegree,		request.m_setHeadingDegree},
				{experimentRecord.actualHeadingDegree,	request.m_actualHeadingDegree},
				{experimentRecord.acousticState,		request.m_acousticState},
				{experimentRecord.targetNum,			request.m_targetNum},
				{experimentRecord.underwaterTargetNum,	request.m_underwaterTargetNum},
				{experimentRecord.underwaterTargetInfo, request.m_underwaterTargetInfo},
				{experimentRecord.maxDepth,				request.m_maxDepth},
				{experimentRecord.profileIndex,			request.m_profileIndex},
				{experimentRecord.profileLength,		request.m_profileLength},
				{experimentRecord.profileDistance,		request.m_profileDistance},
			});

		QSqlQuery query(m_database->sqlDatabase());

		if (query.exec(rps.sql())) {
			if (query.numRowsAffected() > 0) {
				response.m_createResult = true;
			}
		}
		else {
			response.m_errorInfo = Datastruct::SQL_EXECUTE_ERROR;
		}

		return response;
	}
	
	Datastruct::LoadAllExperimentRecordsResponse DataProcessCenter::processExperimentRecordList(int clientId, const Datastruct::LoadAllExperimentRecordsRequest & request)
	{
		Datastruct::LoadAllExperimentRecordsResponse response;

		Table::ExperimentRecordEntity experimentRecord;

		Base::RSelect rs(experimentRecord.table);
		if (!request.m_platformId.isEmpty()) {
			rs.orderBy(experimentRecord.table, experimentRecord.floatingTime, Base::SuperCondition::DESC)
				.createCriteria()
				.add(Base::Restrictions::eq(experimentRecord.taskId, request.m_taskId))
				.add(Base::Restrictions::eq(experimentRecord.platformId, request.m_platformId));
		}
		else
		{
			rs.orderBy(experimentRecord.table, experimentRecord.floatingTime, Base::SuperCondition::DESC)
				.createCriteria()
				.add(Base::Restrictions::eq(experimentRecord.taskId, request.m_taskId));
		}


		rs.limit(request.m_offsetIndex, request.m_limitIndex);

		QSqlQuery query(m_database->sqlDatabase());

		if (query.exec(rs.sql())) {
			while (query.next()) {

				Datastruct::ExperimentRecordEntityData data;
				data.id = query.value(experimentRecord.id).toString();
				data.taskId = query.value(experimentRecord.taskId).toString();
				data.platformId = query.value(experimentRecord.platformId).toString();
				data.floatingTime = query.value(experimentRecord.floatingTime).toDateTime().toString(TIME_FORMAT);;
				data.lon = query.value(experimentRecord.lon).toDouble();
				data.lat = query.value(experimentRecord.lat).toDouble();
				data.setHeadingDegree = query.value(experimentRecord.setHeadingDegree).toDouble();
				data.actualHeadingDegree = query.value(experimentRecord.actualHeadingDegree).toDouble();
				data.acousticState = query.value(experimentRecord.acousticState).toInt();
				data.targetNum = query.value(experimentRecord.targetNum).toInt();
				data.underwaterTargetNum = query.value(experimentRecord.underwaterTargetNum).toInt();
				data.underwaterTargetInfo = query.value(experimentRecord.underwaterTargetInfo).toString();
				data.maxDepth = query.value(experimentRecord.maxDepth).toDouble();
				data.profileIndex = query.value(experimentRecord.profileIndex).toInt();
				data.profileLength = query.value(experimentRecord.profileLength).toDouble();
				data.profileDistance = query.value(experimentRecord.profileDistance).toDouble();

				response.m_experimentRecordInfos.append(data);
			}

			Base::RSelect rst(experimentRecord.table);
			rst.select(experimentRecord.table).
				createCriteria().
				add(Base::Restrictions::eq(experimentRecord.taskId, request.m_taskId));
			rst.count();
			if (query.exec(rst.sql())) {
				if (query.next()) {
					response.m_experimentRecordCount = query.value(0).toInt();
				}
			}
		}
		return response;
	}

	Datastruct::ExperimentRecordDeleteResponse DataProcessCenter::processExperimentRecordDelete(int clientId, const Datastruct::ExperimentRecordDeleteRequest & request)
	{
		Datastruct::ExperimentRecordDeleteResponse response;

		Table::ExperimentRecordEntity experimentRecord;

		Base::RDelete rde(experimentRecord.table);
		rde.createCriteria()
			.add(Base::Restrictions::eq(experimentRecord.id, request.m_id));

		QSqlQuery query(m_database->sqlDatabase());

		if (query.exec(rde.sql())) {
			response.m_deleteResult = true;
		}
		else {
			response.m_errorInfo = Datastruct::SQL_EXECUTE_ERROR;
		};

		return response;
	}

	Datastruct::ExperimentRecordModifyResponse DataProcessCenter::processExperimentRecordModify(int clientId, const Datastruct::ExperimentRecordModifyRequest & request)
	{
		Datastruct::ExperimentRecordModifyResponse response;

		Table::ExperimentRecordEntity experimentRecord;

		Base::RSelect rs(experimentRecord.table);
		rs.select(experimentRecord.table)
			.createCriteria()
			.add(Base::Restrictions::eq(experimentRecord.id, request.m_id));

		QSqlQuery query(m_database->sqlDatabase());

		if (query.exec(rs.sql())) {
			if (query.numRowsAffected() < 0) {
				response.m_errorInfo = Datastruct::SQL_EXECUTE_ERROR;
			}
			else
			{
				Base::RUpdate rud(experimentRecord.table);
				rud.update(experimentRecord.table, {
					{experimentRecord.floatingTime,			request.m_floatingTime},
					{experimentRecord.lon,					request.m_lon},
					{experimentRecord.lat,					request.m_lat},
					{experimentRecord.setHeadingDegree,		request.m_setHeadingDegree},
					{experimentRecord.actualHeadingDegree,	request.m_actualHeadingDegree},
					{experimentRecord.acousticState,		request.m_acousticState},
					{experimentRecord.targetNum,			request.m_targetNum},
					{experimentRecord.underwaterTargetNum,	request.m_underwaterTargetNum},
					{experimentRecord.underwaterTargetInfo, request.m_underwaterTargetInfo},
					{experimentRecord.maxDepth,				request.m_maxDepth},
					{experimentRecord.profileIndex,			request.m_profileIndex},
					{experimentRecord.profileLength,		request.m_profileLength},
					{experimentRecord.profileDistance,		request.m_profileDistance},
					})
					.createCriteria()
					.add(Base::Restrictions::eq(experimentRecord.id, request.m_id));

				QSqlQuery query(m_database->sqlDatabase());

				if (query.exec(rud.sql())) {
					if (query.numRowsAffected()) {
						response.m_modifyResult = true;
					}
					else {
						response.m_errorInfo = Datastruct::NO_FINDDATA;
					}
				}
			}
		}
		else
		{
			response.m_errorInfo = Datastruct::SQL_EXECUTE_ERROR;
		}

		return response;
	}

	Datastruct::DetectPlatformCreateResponse DataProcessCenter::processDetectPlatformCreate(int clientId, const Datastruct::DetectPlatformCreateRequest & request)
	{
		Datastruct::DetectPlatformCreateResponse  response;

		Table::DetectPlatformEntity detectPlatform;

		Base::RPersistence rps(detectPlatform.table);
		rps.insert({
				{detectPlatform.id,				request.m_id},
				{detectPlatform.name,			request.m_name},
			});

		QSqlQuery query(m_database->sqlDatabase());

		if (query.exec(rps.sql())) {
			if (query.numRowsAffected() > 0) {
				response.m_createResult = true;
			}
		} else {
			response.m_errorInfo = Datastruct::SQL_EXECUTE_ERROR;
		}

		return response;
	}

	Datastruct::LoadAllDetectPlatformsResponse DataProcessCenter::processDetectPlatformList(int clientId, const Datastruct::LoadAllDetectPlatformsRequest & request)
	{
		Datastruct::LoadAllDetectPlatformsResponse  response;
		Table::DetectPlatformEntity detectPlatform;

		Base::RSelect rs(detectPlatform.table);
		rs.select(detectPlatform.table);

		QSqlQuery query(m_database->sqlDatabase());

		if (query.exec(rs.sql())) {
			while (query.next()) {
				Datastruct::DetectPlatformEntityData data;
				data.id = query.value(detectPlatform.id).toInt();
				data.name = query.value(detectPlatform.name).toString();
				response.m_detectPlatformInfos.append(data);
			}

			Base::RSelect rst(detectPlatform.table);
			rst.count();

			if (query.exec(rst.sql())) {
				if (query.next()) {
					response.m_detectPlatformCount = query.value(0).toInt();
				}
			}
		} 
		return response;
	}

	Datastruct::DetectPlatformDeleteResponse DataProcessCenter::processDetectPlatformDelete(int clientId, const Datastruct::DetectPlatformDeleteRequest & request)
	{
		Datastruct::DetectPlatformDeleteResponse response;
		Table::DetectPlatformEntity detectPlatform;

		QSqlQuery query(m_database->sqlDatabase());

		Base::RDelete rde(detectPlatform.table);
		rde.createCriteria()
			.add(Base::Restrictions::eq(detectPlatform.id, request.m_id));

		if (query.exec(rde.sql())) {
			if (query.numRowsAffected()) {
				response.m_deleteResult = true;
			}
			else {
				response.m_errorInfo = Datastruct::SQL_EXECUTE_ERROR;
			}
		}
		return response;
	}

	Datastruct::DetectPlatformModifyResponse DataProcessCenter::processDetectPlatformModify(int clientId, const Datastruct::DetectPlatformModifyRequest & request)
	{
		Datastruct::DetectPlatformModifyResponse response;
		Table::DetectPlatformEntity detectPlatform;

		Base::RSelect rs(detectPlatform.table);
		rs.select(detectPlatform.table)
			.createCriteria()
			.add(Base::Restrictions::eq(detectPlatform.id, request.m_id));

		QSqlQuery query(m_database->sqlDatabase());

		if (query.exec(rs.sql())) {
			if (query.numRowsAffected() < 0) {
				response.m_errorInfo = Datastruct::NO_FINDDATA;
			}
			else{
				Base::RUpdate rud(detectPlatform.table);
				rud.update(detectPlatform.table, {
					{detectPlatform.name,	request.m_name},
					})
					.createCriteria()
					.add(Base::Restrictions::eq(detectPlatform.id, request.m_id));

				QSqlQuery query(m_database->sqlDatabase());

				if (query.exec(rud.sql())) {
					if (query.numRowsAffected()) {
						response.m_modifyResult = true;
					}else {
						response.m_errorInfo = Datastruct::SQL_EXECUTE_ERROR;
					}
				}
			}
		} else{
			response.m_errorInfo = Datastruct::SQL_EXECUTE_ERROR;
		}
		return response;
	}

	Datastruct::DetectPlatformSubtypeCreateResponse DataProcessCenter::processDetectPlatformSubtypeCreate(int clientId, const Datastruct::DetectPlatformSubtypeCreateRequest & request)
	{
		Datastruct::DetectPlatformSubtypeCreateResponse response;
		Table::DetectPlatformSubtypeEntity detectPlatformSubtype;

		Base::RPersistence rps(detectPlatformSubtype.table);
		rps.insert({
				{detectPlatformSubtype.id,			request.m_id},
				{detectPlatformSubtype.detectId,	request.m_detectId},
				{detectPlatformSubtype.name,		request.m_name},
			});

		QSqlQuery query(m_database->sqlDatabase());
		if (query.exec(rps.sql())) {
			if (query.numRowsAffected() > 0) {
				response.m_createResult = true;
			}
		}
		else {
			response.m_errorInfo = Datastruct::SQL_EXECUTE_ERROR;
		}
		return response;
	}

	Datastruct::LoadAllDetectPlatformSubtypesResponse DataProcessCenter::processDetectPlatformSubtypeList(int clientId, const Datastruct::LoadAllDetectPlatformSubtypesRequest & request)
	{
		Datastruct::LoadAllDetectPlatformSubtypesResponse response;
		Table::DetectPlatformSubtypeEntity detectPlatformSubtype;

		Base::RSelect rs(detectPlatformSubtype.table);
		rs.orderBy(detectPlatformSubtype.table, detectPlatformSubtype.id, Base::SuperCondition::DESC)
			.createCriteria()
			.add(Base::Restrictions::eq(detectPlatformSubtype.detectId, request.m_detectId));

		QSqlQuery query(m_database->sqlDatabase());

		if (query.exec(rs.sql())) {
			while (query.next()) {
				Datastruct::DetectPlatformSubtypeEntityData data;
				data.id = query.value(detectPlatformSubtype.id).toInt();
				data.detectId = query.value(detectPlatformSubtype.detectId).toInt();
				data.name = query.value(detectPlatformSubtype.name).toString();
				response.m_detectPlatformSubtypeInfos.append(data);
			}
			response.m_detectId = request.m_detectId;
			response.m_detectPlatformSubtypeCount = response.m_detectPlatformSubtypeInfos.size();
		}
		return response;
	}

	Datastruct::DetectPlatformSubtypeDeleteResponse DataProcessCenter::processDetectPlatformSubtypeDelete(int clientId, const Datastruct::DetectPlatformSubtypeDeleteRequest & request)
	{
		Datastruct::DetectPlatformSubtypeDeleteResponse response;
		Table::DetectPlatformSubtypeEntity detectPlatformSubtype;

		QSqlQuery query(m_database->sqlDatabase());

		Base::RDelete rde(detectPlatformSubtype.table);
		rde.createCriteria()
			.add(Base::Restrictions::eq(detectPlatformSubtype.name, request.m_detectId));

		if (query.exec(rde.sql())) {
			if (query.numRowsAffected()) {
				response.m_deleteResult = true;
			}
			else {
				response.m_errorInfo = Datastruct::SQL_EXECUTE_ERROR;
			}
		}
		return response;
	}

	Datastruct::DetectPlatformSubtypeModifyResponse DataProcessCenter::processDetectPlatformSubtypeModify(int clientId, const Datastruct::DetectPlatformSubtypeModifyRequest & request)
	{
		Datastruct::DetectPlatformSubtypeModifyResponse response;
		Table::DetectPlatformSubtypeEntity detectPlatformSubtype;
		Base::RSelect rs(detectPlatformSubtype.table);
		rs.select(detectPlatformSubtype.table)
			.createCriteria()
			.add(Base::Restrictions::eq(detectPlatformSubtype.id, request.m_id));

		QSqlQuery query(m_database->sqlDatabase());

		if (query.exec(rs.sql())) {
			if (query.numRowsAffected() < 0) {
				response.m_errorInfo = Datastruct::NO_FINDDATA;
			}
			else {
				Base::RUpdate rud(detectPlatformSubtype.table);
				rud.update(detectPlatformSubtype.table, {
					{detectPlatformSubtype.detectId,	request.m_detectId},
					{detectPlatformSubtype.name,		request.m_name},
					})
					.createCriteria()
					.add(Base::Restrictions::eq(detectPlatformSubtype.id, request.m_id));

				QSqlQuery query(m_database->sqlDatabase());

				if (query.exec(rud.sql())) {
					if (query.numRowsAffected()) {
						response.m_modifyResult = true;
					}
					else {
						response.m_errorInfo = Datastruct::SQL_EXECUTE_ERROR;
					}
				}
			}
		}
		else {
			response.m_errorInfo = Datastruct::SQL_EXECUTE_ERROR;
		}
		return response;
	}

	Datastruct::TargetCreateResponse DataProcessCenter::processTargetCreate(int clientId, const Datastruct::TargetCreateRequest & request)
	{
		Datastruct::TargetCreateResponse response;

		Table::TargetDataEntity target;
		Base::RPersistence rps(target.table);
		rps.insert({
				{target.id,			request.m_id},
				{target.name,		request.m_name},
				{target.type,		request.m_type},
				{target.createTime,	QDateTime::fromString(request.m_creanTime, TIME_FORMAT)},
			});

		QSqlQuery query(m_database->sqlDatabase());
		if (query.exec(rps.sql())) {
			if (query.numRowsAffected() > 0) {
				response.m_createResult = true;
			}
		}
		else {
			response.m_errorInfo = Datastruct::SQL_EXECUTE_ERROR;
		}
		return response;
	}

	Datastruct::LoadAllTargetResponse DataProcessCenter::processTargetList(int clientId, const Datastruct::LoadAllTargetRequest & request)
	{
		Datastruct::LoadAllTargetResponse response;
		Table::TargetDataEntity target;
		Base::RSelect rs(target.table);
		rs.select(target.table);
		QSqlQuery query(m_database->sqlDatabase());
		if (query.exec(rs.sql())) {
			while (query.next()) {
				Datastruct::TargetEntityData data;
				data.id = query.value(target.id).toString();
				data.name = query.value(target.name).toString();
				data.type = query.value(target.type).toInt();
				data.createTime = query.value(target.createTime).toDateTime().toString(TIME_FORMAT);
				response.m_targetInfos.append(data);
			}
			Base::RSelect rst(target.table);
			rst.count();
			if (query.exec(rst.sql())) {
				if (query.next()) {
					response.m_targetCount = query.value(0).toInt();
				}
			}
		}
		return response;
	}

	Datastruct::TargetDeleteResponse DataProcessCenter::processTargetDelete(int clientId, const Datastruct::TargetDeleteRequest & request)
	{
		Datastruct::TargetDeleteResponse response;
		Table::TargetDataEntity target;

		QSqlQuery query(m_database->sqlDatabase());

		Base::RDelete rde(target.table);
		rde.createCriteria()
			.add(Base::Restrictions::eq(target.id, request.m_id));

		if (query.exec(rde.sql())) {
			if (query.numRowsAffected()) {
				response.m_deleteResult = true;
			} else {
				response.m_errorInfo = Datastruct::SQL_EXECUTE_ERROR;
			}
		}
		return response;
	}

	Datastruct::TargetModifyResponse DataProcessCenter::processTargetModify(int clientId, const Datastruct::TargetModifyRequest & request)
	{
		Datastruct::TargetModifyResponse response;
		Table::TargetDataEntity target;
		Base::RSelect rs(target.table);
		rs.select(target.table)
			.createCriteria()
			.add(Base::Restrictions::eq(target.id, request.m_id));
		QSqlQuery query(m_database->sqlDatabase());
		if (query.exec(rs.sql())) {
			if (query.numRowsAffected() < 0) {
				response.m_errorInfo = Datastruct::NO_FINDDATA;
			}
			else {
				Base::RUpdate rud(target.table);
				rud.update(target.table, {
						{target.name,		request.m_name},
						{target.createTime,	QDateTime::fromString(request.m_createTime, TIME_FORMAT)},
						{target.type,		request.m_type},
					})
					.createCriteria()
					.add(Base::Restrictions::eq(target.id, request.m_id));
				QSqlQuery query(m_database->sqlDatabase());

				if (query.exec(rud.sql())) {
					if (query.numRowsAffected()) {
						response.m_modifyResult = true;
					}
					else {
						response.m_errorInfo = Datastruct::SQL_EXECUTE_ERROR;
					}

				}
			}
		}
		else {
			response.m_errorInfo = Datastruct::SQL_EXECUTE_ERROR;
		}
		return response;
	}

	Datastruct::AISDataCreateResponse DataProcessCenter::processAISCreate(int clientId, const Datastruct::AISDataCreateRequest & request)
	{
		Datastruct::AISDataCreateResponse response;

		Table::AISDataEntity aisData;
		Base::RPersistence rps(aisData.table);
		rps.insert({
				{aisData.id,		request.m_id},
				{aisData.targetId,	request.m_targetId},
				{aisData.mmsi,		request.m_mmsi},
				{aisData.time,		request.m_time},
				{aisData.lon,		request.m_lon},
				{aisData.lat,		request.m_lat},
				{aisData.course,	request.m_course},
				{aisData.truehead,	request.m_truehead},
				{aisData.name,		request.m_name},
				{aisData.shipType,	request.m_shipType},
				{aisData.shipImo,	request.m_shipImo},
				{aisData.navStatus,	request.m_navStatus},
				{aisData.speed,		request.m_speed},
				{aisData.eta,		QDateTime::fromString(request.m_eta, TIME_FORMAT)},
				{aisData.dest,		request.m_dest},
				{aisData.length,	request.m_length},
				{aisData.width,		request.m_width},
				{aisData.callsign,	request.m_callsign},
				{aisData.flag,		request.m_flag},
				{aisData.buildDate,	QDateTime::fromString(request.m_buildDate, TIME_FORMAT)},
				{aisData.port,		request.m_port},
			});

		QSqlQuery query(m_database->sqlDatabase());
		if (query.exec(rps.sql())) {
			if (query.numRowsAffected() > 0) {
				response.m_createResult = true;
			}
		}
		else {
			response.m_errorInfo = Datastruct::SQL_EXECUTE_ERROR;
		}
		return response;
	}

	Datastruct::LoadAllAISDatasResponse DataProcessCenter::processAISDataList(int clientId, const Datastruct::LoadAllAISDataRequest & request)
	{
		Datastruct::LoadAllAISDatasResponse response;

		Table::AISDataEntity aisData;

		Base::RSelect rs(aisData.table);
		rs.select(aisData.table)
			.limit(request.m_offsetIndex, request.m_limitIndex);

		QSqlQuery query(m_database->sqlDatabase());

		if (query.exec(rs.sql())) {
			while (query.next()) {

				Datastruct::AisEntityData data;
				data.id = query.value(aisData.id).toString();
				data.targetId = query.value(aisData.targetId).toString();
				data.mmsi = query.value(aisData.mmsi).toInt();
				data.time = query.value(aisData.time).toInt();
				data.lon = query.value(aisData.lon).toDouble();
				data.lat = query.value(aisData.lat).toDouble();
				data.course = query.value(aisData.course).toDouble();
				data.truehead = query.value(aisData.truehead).toInt();
				data.name = query.value(aisData.name).toString();
				data.shipType = query.value(aisData.shipType).toInt();
				data.shipImo = query.value(aisData.shipImo).toInt();
				data.navStatus = query.value(aisData.navStatus).toInt();
				data.speed = query.value(aisData.speed).toDouble();
				data.eta = query.value(aisData.eta).toDateTime().toString(TIME_FORMAT);
				data.dest = query.value(aisData.dest).toString();
				data.length = query.value(aisData.length).toDouble();
				data.width = query.value(aisData.width).toDouble();
				data.callsign = query.value(aisData.callsign).toString();
				data.flag = query.value(aisData.flag).toString();
				data.buildDate = query.value(aisData.buildDate).toDateTime().toString(TIME_FORMAT);
				data.port = query.value(aisData.port).toString();

				response.m_aisDataInfos.append(data);
			}

			Base::RSelect rst(aisData.table);
			rst.select(aisData.table);
			rst.count();
			if (query.exec(rst.sql())) {
				if (query.next()) {
					response.m_aisDataCount = query.value(0).toInt();
				}
			}
		}
		return response;
	}

	Datastruct::AISDataByConditionResponse DataProcessCenter::processAISDataByCondition(int clientId, const Datastruct::AISDataByConditionRequest & request)
	{
		return Datastruct::AISDataByConditionResponse();
	}

	Datastruct::AISDataDeleteResponse DataProcessCenter::processAISDataDelete(int clientId, const Datastruct::AISDataDeleteRequest & request)
	{
		Datastruct::AISDataDeleteResponse response;
		Table::AISDataEntity aisData;
		QSqlQuery query(m_database->sqlDatabase());

		Base::RDelete rde(aisData.table);
		rde.createCriteria()
			.add(Base::Restrictions::eq(aisData.id, request.m_id));

		if (query.exec(rde.sql())) {
			if (query.numRowsAffected()) {
				response.m_deleteResult = true;
			}
			else {
				response.m_errorInfo = Datastruct::SQL_EXECUTE_ERROR;
			}
		}
		return response;
	}

	Datastruct::AISDataModifyResponse DataProcessCenter::processAISDataModify(int clientId, const Datastruct::AISDataModifyRequest & request)
	{
		Datastruct::AISDataModifyResponse response;
		Table::AISDataEntity aisData;
		Base::RSelect rs(aisData.table);
		rs.select(aisData.table)
			.createCriteria()
			.add(Base::Restrictions::eq(aisData.id, request.m_id));
		QSqlQuery query(m_database->sqlDatabase());
		if (query.exec(rs.sql())) {
			if (query.numRowsAffected() < 0) {
				response.m_errorInfo = Datastruct::SQL_EXECUTE_ERROR;
			}
			else
			{
				Base::RUpdate rud(aisData.table);
				rud.update(aisData.table, {
					{aisData.targetId,	request.m_targetId},
					{aisData.mmsi,		request.m_mmsi},
					{aisData.time,		request.m_time},
					{aisData.lon,		request.m_lon},
					{aisData.lat,		request.m_lat},
					{aisData.course,	request.m_course},
					{aisData.truehead,	request.m_truehead},
					{aisData.name,		request.m_name},
					{aisData.shipType,	request.m_shipType},
					{aisData.shipImo,	request.m_shipImo},
					{aisData.navStatus,	request.m_navStatus},
					{aisData.speed,		request.m_speed},
					{aisData.eta,		QDateTime::fromString(request.m_eta, TIME_FORMAT)},
					{aisData.dest,		request.m_dest},
					{aisData.length,	request.m_length},
					{aisData.width,		request.m_width},
					{aisData.callsign,	request.m_callsign},
					{aisData.flag,		request.m_flag},
					{aisData.buildDate,	QDateTime::fromString(request.m_buildDate, TIME_FORMAT)},
					{aisData.port,		request.m_port},
					})
					.createCriteria()
					.add(Base::Restrictions::eq(aisData.id, request.m_id));

				QSqlQuery query(m_database->sqlDatabase());

				if (query.exec(rud.sql())) {
					if (query.numRowsAffected()) {
						response.m_modifyResult = true;
					}
					else {
						response.m_errorInfo = Datastruct::NO_FINDDATA;
					}
				}
			}
		}
		else
		{
			response.m_errorInfo = Datastruct::SQL_EXECUTE_ERROR;
		}

		return response;
	}

} //namespace Related 