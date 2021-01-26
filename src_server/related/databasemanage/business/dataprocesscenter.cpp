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

		Base::RSelect rs(task.table);
		rs.select(task.table)
			.createCriteria()
			.add(Base::Restrictions::eq(task.id, request.taskId));

		QSqlQuery query(m_database->sqlDatabase());

		do {
			if (query.exec(rs.sql())) {
				if (query.numRowsAffected() > 0) {
					response.m_createResult = false;
					response.m_errorInfo = QStringLiteral("该记录存在");
					break;
				}
			}

			Base::RPersistence rps(task.table);
			rps.insert({
					{task.id,		request.taskId},
					{task.name,		request.taskName},
					{task.startTime,request.startTime},
					{task.endTime,	request.endTime},
					{task.location, request.location},
					{task.lon,		request.lon},
					{task.lat,		request.lat},
					{task.description, request.description},
					{task.detectPlatform, request.detectPlatform},
				});

			if (query.exec(rps.sql())) {
				if (query.numRowsAffected() > 0) {
					response.m_createResult = true;
				}
			}
			else {
				response.m_errorInfo = QStringLiteral("保存数据失败.");
			}

		} while (0);

		return response;
	}

	Datastruct::LoadAllTaskResponse DataProcessCenter::processTaskList(int clientId, const Datastruct::LoadAllTaskRequest & request)
	{
		Datastruct::LoadAllTaskResponse response;

		Table::TaskEntity task;

		Base::RSelect rs(task.table);
		rs.select(task.table)
			.limit(request.m_offsetIndex, request.m_limitIndex);

		QSqlQuery query(m_database->sqlDatabase());

		if (query.exec(rs.sql())) {
			while (query.next()) {

				Datastruct::TaskEntityData data;
				data.id = query.value(task.id).toString();
				data.taskName = query.value(task.name).toString();
				data.startTime = query.value(task.startTime).toDateTime().toString(TIME_FORMAT);
				data.endTime = query.value(task.endTime).toDateTime().toString(TIME_FORMAT);
				data.location = query.value(task.location).toString();
				data.lon = query.value(task.lon).toDouble();
				data.lat = query.value(task.lat).toDouble();
				data.description = query.value(task.description).toString();
				data.detectPlatform = query.value(task.detectPlatform).toString();

				response.m_taskInfos.append(data);
			}

			Base::RSelect rst(task.table);
			rst.count();

			qDebug() << rst.sql();

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

		QSqlQuery query(m_database->sqlDatabase());

		QString deleteSql = QString("DELETE FROM %1 WHERE %2 = '%3'").arg(task.table).arg(task.id).arg(request.taskId);
		if (query.exec(deleteSql)) {
			response.m_deleteResult = true;
		}
		else
		{
			response.m_deleteResult = false;
			response.m_errorInfo = QStringLiteral("删除失败");
		};
		return response;
	}

	Datastruct::DutyRecordCreateResponse DataProcessCenter::processDutyRecordCreate(int clientId, const Datastruct::DutyRecordCreateRequest & request)
	{
		Datastruct::DutyRecordCreateResponse response;

		Table::DutyRecordEntity dutyRecord;

		Base::RSelect rs(dutyRecord.table);
		rs.select(dutyRecord.table)
			.createCriteria()
			.add(Base::Restrictions::eq(dutyRecord.id, request.taskId));

		QSqlQuery query(m_database->sqlDatabase());

		do {
			if (query.exec(rs.sql())) {
				if (query.numRowsAffected() > 0) {
					response.m_createResult = false;
					response.m_errorInfo = QStringLiteral("该记录存在");
					break;
				}
			}

			Base::RPersistence rps(dutyRecord.table);
			rps.insert({
					{dutyRecord.id,		request.id},
					{dutyRecord.taskId,		request.taskId},
					{dutyRecord.createTime, QDateTime::currentDateTime()},
					{dutyRecord.description, request.description},
					{dutyRecord.seaCondition, request.seaCondition},
				});

			if (query.exec(rps.sql())) {
				if (query.numRowsAffected() > 0) {
					response.m_createResult = true;
				}
			}
			else {
				response.m_errorInfo = QStringLiteral("保存数据失败.");
			}

		} while (0);

		return response;
	}

	Datastruct::LoadAllDutyRecordResponse DataProcessCenter::processDutyRecordList(int clientId, const Datastruct::LoadAllDutyRecordRequest & request)
	{
		Datastruct::LoadAllDutyRecordResponse response;

		Table::DutyRecordEntity dutyRecord;

		Base::RSelect rs(dutyRecord.table);
		rs.select(dutyRecord.table)
			.limit(request.m_offsetIndex, request.m_limitIndex);

		QSqlQuery query(m_database->sqlDatabase());

		if (query.exec(rs.sql())) {
			while (query.next()) {

				Datastruct::DutyRecordEntityData data;
				data.id = query.value(dutyRecord.id).toString();
				data.taskId = query.value(dutyRecord.taskId).toString();
				data.createTime = query.value(dutyRecord.createTime).toDateTime().toString(TIME_FORMAT);
				data.description = query.value(dutyRecord.description).toString();
				data.seaCondition = query.value(dutyRecord.seaCondition).toString();
				response.m_dutyRecordInfos.append(data);
			}

			Base::RSelect rst(dutyRecord.table);
			rst.count();

			qDebug() << rst.sql();

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

		return response;
	}
	Datastruct::ExperimentRecordCreateResponse DataProcessCenter::processExperimentRecordCreate(int clientId, const Datastruct::ExperimentRecordCreateRequest & request)
	{
		Datastruct::ExperimentRecordCreateResponse response;

		Table::ExperimentRecordEntity experimentRecord;

		Base::RSelect rs(experimentRecord.table);
		rs.select(experimentRecord.table)
			.createCriteria()
			.add(Base::Restrictions::eq(experimentRecord.id, request.taskId));

		QSqlQuery query(m_database->sqlDatabase());

		do {
			if (query.exec(rs.sql())) {
				if (query.numRowsAffected() > 0) {
					response.m_createResult = false;
					response.m_errorInfo = QStringLiteral("该记录存在");
					break;
				}
			}

			Base::RPersistence rps(experimentRecord.table);
			rps.insert({
					{experimentRecord.id,			request.id},
					{experimentRecord.taskId,		request.taskId},
					{experimentRecord.platformId,	request.platformId},
					{experimentRecord.lon,			QString::number(request.lon) },
					{experimentRecord.lat,			QString::number(request.lat)},
					{experimentRecord.seaCondition, request.seaCondition},
					{experimentRecord.floatingTime, request.floatingTime},
				});

			qDebug() << rps.sql();

			if (query.exec(rps.sql())) {
				if (query.numRowsAffected() > 0) {
					response.m_createResult = true;
				}
			}
			else {
				response.m_errorInfo = QStringLiteral("保存数据失败.");
			}

		} while (0);

		return response;
	}
	Datastruct::LoadAllExperimentRecordsResponse DataProcessCenter::processExperimentRecordList(int clientId, const Datastruct::LoadAllExperimentRecordsRequest & request)
	{
		Datastruct::LoadAllExperimentRecordsResponse response;

		Table::ExperimentRecordEntity experimentRecord;

		Base::RSelect rs(experimentRecord.table);
		rs.select(experimentRecord.table)
			.limit(request.m_offsetIndex, request.m_limitIndex);

		QSqlQuery query(m_database->sqlDatabase());

		if (query.exec(rs.sql())) {
			while (query.next()) {

				Datastruct::ExperimentRecordEntityData data;
				data.id = query.value(experimentRecord.id).toString();
				data.taskId = query.value(experimentRecord.taskId).toString();
				data.platformId = query.value(experimentRecord.platformId).toString();
				data.lon = query.value(experimentRecord.lon).toDouble();
				data.lat = query.value(experimentRecord.lat).toDouble();
				data.seaCondition = query.value(experimentRecord.seaCondition).toString();
				data.floatingTime = query.value(experimentRecord.floatingTime).toString();

				response.m_listInfos.append(data);
			}

			Base::RSelect rst(experimentRecord.table);
			rst.count();

			qDebug() << rst.sql();

			if (query.exec(rst.sql())) {
				if (query.next()) {
					response.m_count = query.value(0).toInt();
				}
			}
		}
		return response;
	}
	Datastruct::ExperimentRecordDeleteResponse DataProcessCenter::processExperimentRecordDelete(int clientId, const Datastruct::ExperimentRecordDeleteRequest & request)
	{
		Datastruct::ExperimentRecordDeleteResponse response;

		Table::ExperimentRecordEntity experimentRecord;

// 		Base::RDelete rs(experimentRecord.table);
// 		r(experimentRecord.table)
// 			.limit(request.m_offsetIndex, request.m_limitIndex);
// 
// 		QSqlQuery query(m_database->sqlDatabase());

		return response;
	}

} //namespace Related 