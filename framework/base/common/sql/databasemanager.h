/*!
 *  @brief     数据库管理器
 *  @details   负责创建数据库连接
 *  @author    wey
 *  @version   1.0
 *  @date      2018.01.XX
 *  @warning
 *  @copyright NanJing RenGu.
 */
#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QString>
#include <QSqlDriver>
#include <QList>

#include <commondefines/databaseprotocol.h>
#include "database.h"
#include "../../base_global.h"

namespace Base {

	class BASESHARED_EXPORT DatabaseManager
	{
	public:
		static DatabaseManager * instance();

		/*! 
		 * @brief 设置数据库基本配置信息
		 * @param configInfo 包括类型、连接地址、用户名等配置信息
		 */
		void setDatabaseInfo(DB::DatabaseConfigInfo configInfo);

		bool hasFeature(QSqlDriver::DriverFeature feature);
		QStringList availableDrivers();

		static bool testConnection(DB::DatabaseConfigInfo & info);

		/*! 
		 * @brief 获取新数据库连接，并指定连接名
		 * @param connectionName 数据库连接标识，不能重复
		 * @return 若创建并打开成功，则返回创建的指针；否则返回nullptr
		 */
		Database * newDatabase(QString connectionName = "");

	private:
		DatabaseManager();

	private:
		static DatabaseManager * m_instance;

		DB::DatabaseConfigInfo dbConfigInfo;
	};

} //namespace Base

#endif // DATABASEMANAGER_H
