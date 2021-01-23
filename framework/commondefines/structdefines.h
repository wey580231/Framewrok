/*!
 * @brief     数据协议   
 * @author    wey
 * @version   1.0
 * @date      2021.01.23 15:17:10
 * @warning
 * @copyright NanJing RenGu.
 * @note
 */
 #pragma once

#include <QObject>
#include <QString>
#include <QDateTime>

namespace Datastruct {

	enum UserPrivilege {
		ReadOnly = 0x0001,			/*!< 只可以查询，不可编辑 */
		ReadWrite = 0x0002,			/*!< 可读写(包括更新) */
		DeleteAble = 0x0004,		/*!< 可删除 */
		AllPrivilege = ReadWrite | DeleteAble
	};

	/*!
	 * @brief 单个用户实体数据结构
	 * @details 
	 */
	struct UserEntityData {
		UserEntityData() :id(0), isManager(false){}

		int id;					/*!< 数据库ID */
		QString name;			/*!< 用户名 */
		QString password;		/*!< 密码 */
		QString registTime;		/*!< 注册时间 */
		int privilege;			/*!< 权限 */
		bool isManager;			/*!< 是否为管理员 */
	};

} //namespace Datastruct 