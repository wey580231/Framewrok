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
					response.m_errorInfo = QStringLiteral("账户密码错误.");
				}
				else {
					response.m_loginResult = true;

					response.m_userInfo.id = query.value(user.id).toInt();
					response.m_userInfo.name = query.value(user.userName).toString();
					response.m_userInfo.password = query.value(user.userPassword).toString();
					response.m_userInfo.registTime = query.value(user.regitstTime).toDateTime().toString(TIME_FORMAT);
					response.m_userInfo.privilege = query.value(user.privilege).toInt();
					response.m_userInfo.isManager = query.value(user.superManage).toBool();
				}
			}
			else {
				response.m_errorInfo = QStringLiteral("账户未注册.");
			}
		}
		else {
			response.m_errorInfo = QStringLiteral("数据访问失败.");
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
					response.m_errorInfo = QStringLiteral("用户名已存在.");
					break;
				}
			}


			Base::RPersistence rps(user.table);
			rps.insert({
					{user.userName,request.m_name},
					{user.userPassword,request.m_password},
					{user.regitstTime,QDateTime::currentDateTime()},
					{user.privilege,0},
					{user.superManage,0}
				});

			if (query.exec(rps.sql())) {
				if (query.numRowsAffected() > 0) {
					response.m_loginResult = true;
				}
			}
			else {
				response.m_errorInfo = QStringLiteral("保存数据失败.");
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
				data.privilege = query.value(user.privilege).toInt();
				data.isManager = query.value(user.superManage).toBool();

				response.m_userInfos.append(data);
			}

			Base::RSelect rst(user.table);
			rst.count();

			qDebug() << rst.sql();

			if (query.exec(rst.sql())) {
				if (query.next()) {
					response.m_userCount = query.value(0).toInt();
				}
			}
		}

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
				});

			qDebug() << "____" <<rps.sql();

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

} //namespace Related 