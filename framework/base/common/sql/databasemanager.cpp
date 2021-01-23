#include "databasemanager.h"

#include <QSqlError>

#include <functional>

#include "../../Util/rlog.h"
#include "../../Util/rutil.h"

namespace Base {

	DatabaseManager * DatabaseManager::m_instance = nullptr;

	DatabaseManager::DatabaseManager()
	{
	}

	DatabaseManager * DatabaseManager::instance()
	{
		if (m_instance == nullptr)
			m_instance = new DatabaseManager;

		return m_instance;
	}

	void DatabaseManager::setDatabaseInfo(DB::DatabaseConfigInfo configInfo)
	{
		dbConfigInfo = configInfo;
	}

	/*!
	 * @brief 测试当前设置是否正常
	 * @param[in] info 新的数据库连接信息
	 * @return true表示测试通过，false表示测试失败
	 */
	bool DatabaseManager::testConnection(DB::DatabaseConfigInfo &info)
	{
		bool opened = false;
		Database db(info.dbType, Base::RUtil::UUID());
		if (db.init())
		{
			db.setHostName(info.hostName);
			db.setDatabaseName(info.dbName);
			db.setUserName(info.dbUser);
			db.setPassword(info.dbPass);
			if (info.port > 0)
				db.setPort(info.port);

			opened = db.open();

			db.close();
		}
		return opened;
	}

	Database *DatabaseManager::newDatabase(QString connectionName)
	{
		Database * tmpDb = new Database(dbConfigInfo.dbType, connectionName);

		do
		{
			if (!tmpDb->init())
				break;

			tmpDb->setHostName(dbConfigInfo.hostName);
			tmpDb->setDatabaseName(dbConfigInfo.dbName);
			tmpDb->setUserName(dbConfigInfo.dbUser);
			tmpDb->setPassword(dbConfigInfo.dbPass);

			if (dbConfigInfo.port > 0)
				tmpDb->setPort(dbConfigInfo.port);

			if (!tmpDb->open())
				break;

			return tmpDb;

		} while (0);

		delete tmpDb;
		return nullptr;
	}

	/*!
	 * @brief 查询当前数据库是否支持指定的功能
	 * @param[in] feature 待查询的功能
	 * @return 是否支持指定的功能
	 */
	bool DatabaseManager::hasFeature(QSqlDriver::DriverFeature feature)
	{
		static Database db(dbConfigInfo.dbType);
		if (db.init()) {
			return db.sqlDatabase().driver()->hasFeature(feature);
		}
		return false;
	}

	QStringList DatabaseManager::availableDrivers()
	{
		return QSqlDatabase::drivers();
	}

} //namespace Base
