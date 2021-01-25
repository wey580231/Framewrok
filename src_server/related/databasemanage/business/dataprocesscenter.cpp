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

} //namespace Related 