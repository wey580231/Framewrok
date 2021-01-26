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

	/*!
	 * @brief 任务信息表
	*/
	struct TaskEntity : public AbstractTable {
		TaskEntity() :AbstractTable("task_entity") {}

		QString id = "id";
		QString name = "name";
		QString startTime = "start_time";
		QString endTime = "end_time";
		QString location = "location";
		QString lon = "lon";
		QString lat = "lat";
		QString description = "description";
		QString detectPlatform = "detect_platform";
	};

	/*!
	* @brief 值班日志信息表
	*/
	struct DutyRecordEntity : public AbstractTable {
		DutyRecordEntity() :AbstractTable("task_duty_record") {}

		QString id = "id";
		QString taskId = "task_id";
		QString createTime = "create_time";
		QString description = "description";
		QString seaCondition = "sea_condition";
	};

	/*!
	* @brief 试验记录信息表
	*/
	struct ExperimentRecordEntity : public AbstractTable {
		ExperimentRecordEntity() :AbstractTable("task_experiment_record") {}

		QString id = "id";
		QString taskId = "task_id";
		QString platformId = "platform_id";
		QString lon = "lon";
		QString lat = "lat";
		QString seaCondition = "sea_condition";
		QString floatingTime = "floating_time";
	};


}	//namespace Table
