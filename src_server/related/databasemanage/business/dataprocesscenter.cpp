#include "dataprocesscenter.h"

#include <QSqlQuery>
#include <QDateTime>
#include <QDebug>

#include <commondefines\sqltable\tablemapper.h>
#include <base\common\sql\rpersistence.h>

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
				{task.id,		Base::RUtil::UUID()},
				{task.name,		request.taskName},
				{task.startTime,QDateTime::fromString(request.startTime, TIME_FORMAT)},
				{task.endTime,	QDateTime::fromString(request.endTime, TIME_FORMAT)},
				{task.location, request.location},
				{task.lon,		request.lon},
				{task.lat,		request.lat},
				{task.description, request.description},
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
		rs.orderBy(task.table, task.startTime, Base::SuperCondition::DESC)
			.createCriteria()
			.add(Base::Restrictions::eq(task.location, request.location));

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
		// 所有任务起始时间
		Table::TaskEntity task;

		QSqlQuery query(m_database->sqlDatabase());

		Base::RSelect minRst(task.table);
		// 起始时间
		minRst.min(task.startTime);
		if (query.exec(minRst.sql())) {
			if (query.next()) {
				response.allTaskStartTime = query.value(0).toDateTime().toString(TIME_FORMAT);
			}
		}
		// 结束时间
		Base::RSelect maxRst(task.table);
		maxRst.max(task.endTime);
		if (query.exec(maxRst.sql())) {
			if (query.next()) {
				qDebug() << query.value(0).toDateTime().toString(TIME_FORMAT);
				response.allTaskEndTime = query.value(0).toDateTime().toString(TIME_FORMAT);
			}
		}
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
		rs.orderBy(experimentRecord.table, experimentRecord.floatingTime, Base::SuperCondition::DESC)
			.createCriteria()
			.add(Base::Restrictions::eq(experimentRecord.taskId, request.m_taskId));
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
			.add(Base::Restrictions::eq(detectPlatformSubtype.id, request.m_id));

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

} //namespace Related 