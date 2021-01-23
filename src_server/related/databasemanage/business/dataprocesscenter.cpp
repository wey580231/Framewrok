#include "dataprocesscenter.h"

#include <QSqlQuery>
#include <QDateTime>
#include <QDebug>

#include <commondefines\sqltable\tablemapper.h>
#include <base\common\sql\rpersistence.h>

namespace Related {

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
			.add(Base::Restrictions::eq(user.userName,request.m_name))
			.add(Base::Restrictions::eq(user.userPassword, request.m_password));

		QSqlQuery query(m_database->sqlDatabase());

		if (query.exec(rs.sql())) {
			if (query.next()) {
				response.m_loginResult = true;

				response.m_userInfo.id = query.value(user.id).toInt();
				response.m_userInfo.name = query.value(user.userName).toString();
				response.m_userInfo.password = query.value(user.userPassword).toString();
				response.m_userInfo.registTime = query.value(user.regitstTime).toDateTime().toString("yyyy-MM-dd hh:mm:ss");
				response.m_userInfo.privilege = query.value(user.privilege).toInt();
				response.m_userInfo.isManager = query.value(user.superManage).toBool();
			}
			else {
				response.m_errorInfo = QStringLiteral("Î´×¢²á");
			}
		}
		else {
			response.m_errorInfo = QStringLiteral("Êý¾Ý·ÃÎÊÊ§°Ü");
		}
		
		return response;
	}

} //namespace Related 