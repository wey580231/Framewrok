/*!
 * @brief     数据库表字段映射
 * @details   定义各个数据库表以及表字段名,用于在访问数据库编写sql时，减少对字段的硬编码
 * @author    wey
 * @version   1.0
 * @date      2021.01.23 14:08:34
 * @warning
 * @copyright NanJing RenGu.
 * @note
 */
 #pragma once

#include <QObject>
#include <QString>

namespace Table {

	struct AbstractTable {
		AbstractTable(QString tname):table(tname) {}
		QString table;
	};

	/*!
	 * @brief 用户信息表
	 */
	struct UserEntity : public AbstractTable {
		UserEntity() :AbstractTable("user_entity") {}

		QString id = "id";
		QString userName = "user_name";
		QString userPassword = "user_password";
		QString regitstTime = "regist_time";
		QString privilege = "privilege";
		QString superManage = "super_manage";
	};


}	//namespace Table
