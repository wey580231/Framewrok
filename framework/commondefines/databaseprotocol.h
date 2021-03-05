/*!
 * @brief     数据库配置信息   
 * @author    wey
 * @version   1.0
 * @date      2021.01.23 10:51:04
 * @warning
 * @copyright NanJing RenGu.
 * @note
 */
 #pragma once

#include <QObject>

namespace DB{

	/*!
	 *  @brief 支持数据库种类
	 */
	enum DatabaseType
	{
		DB_NONE = 0,
		DB_MYSQL,               /*!< 使用MySql数据库*/
		DB_ORACLE               /*!< 使用Oracle数据库*/
	};

	/*!
	 *  @brief 数据库特征信息
	 *  @note 保存当前使用的数据库中的功能特点
	 */
	struct DBFeature {
		bool lastInsertId;          /*!< 是否支持查询最后插入数据的id */
		bool transactions;          /*!< 是否支持事务 */
	};

	/*!
	 *  @brief 数据库配置信息
	 */
	struct DatabaseConfigInfo
	{
		DatabaseType dbType;		/*!< 数据库类型 */
		QString hostName;			/*!< 主机名 */
		QString dbName;				/*!< 待访问数据库名 */
		QString dbUser;				/*!< 用户名 */
		QString dbPass;				/*!< 密码 */
		ushort port;				/*!< 数据库端口 */
	};

};
