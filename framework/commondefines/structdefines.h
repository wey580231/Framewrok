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
		NonePrivilege = 0x0000,		/*!< 无权限 */
		ReadOnly = 0x0001,			/*!< 只可以查询，不可编辑 */
		ReadWrite = 0x0002,			/*!< 可读写(包括更新) */
		DeleteAble = 0x0004,		/*!< 可删除 */
		AllPrivilege = ReadOnly | ReadWrite | DeleteAble		/*!< 全部权限 */
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
		QString lastLoadTime;	/*!< 最后一次登录时间 */
		int privilege;			/*!< 用户具备的权限，@see UserPrivilege */
		bool isManager;			/*!< 是否为管理员,管理员可以修改普通用户的权限，系统中至少存在一个管理员 */
	};

} //namespace Datastruct 