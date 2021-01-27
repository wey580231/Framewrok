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

	/*!
	 * @brief  单条任务实体数据结构
	 * @details 
	 */
	struct TaskEntityData
	{
		TaskEntityData():lon(0), lat(0){
		}
		QString id;				/*!< 数据库Id */
		QString taskName;		/*!< 任务名称 */
		QString startTime;		/*!< 起始时间 */
		QString endTime;		/*!< 结束时间 */
		QString location;		/*!< 任务地点 */
		double lon;				/*!< 经度 */
		double lat;				/*!< 纬度 */
		QString description;	/*!< 描述 */
		QString detectPlatform;	/*!< 检测平台 */
	};

	/*!
	 * @brief  单条值班日志实体数据结构
	 * @details 
	 */
	struct DutyRecordEntityData {
		DutyRecordEntityData() {

		}
		QString id;				/*!< 数据库ID */
		QString taskId;			/*!< 任务Id */
		QString createTime;		/*!< 创建时间 */
		QString description;	/*!< 描述 */
		QString seaCondition;	/*!< 海况信息 */
	};

	/*!
	 * @brief    单条试验记录实体数据结构
	 * @details
	 */
	struct ExperimentRecordEntityData {
		ExperimentRecordEntityData() :lon(0), lat(0){

		}
		QString id;				/*!< 数据库ID */
		QString taskId;			/*!< 任务Id */
		QString platformId;		/*!< 平台Id */
		double lon;				/*!< 经度 */
		double lat;				/*!< 纬度 */
		QString seaCondition;	/*!< 海况 */
		QString floatingTime;	/*!< 浮动时间 */
	};

} //namespace Datastruct 