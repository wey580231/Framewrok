#include "mainwindow.h"

#include <QHBoxLayout>
#include <QDebug>

#include <commondefines/databaseprotocol.h>
#include <base/util/rutil.h>
#include <base/util/rsingleton.h>
#include <base/common/sql/databasemanager.h>

#include "../net/requestprocessthread.h"
#include "../net/netacceptor.h"

namespace Related {

	MainWindow::MainWindow(QWidget *parent)
		: QWidget(parent)
	{
		init();
		initDatabase();
		initThread();
		initNetwork();
	}

	MainWindow::~MainWindow()
	{
		RSingleton<RequestProcessThread>::instance()->stopMe();
	}

	void MainWindow::init()
	{
		m_stackedWidget = new QStackedWidget();

		QWidget * mainWidget = new QWidget();

		m_stackedWidget->addWidget(mainWidget);

		QHBoxLayout * layout = new QHBoxLayout();
		layout->setContentsMargins(0, 0, 0, 0);
		layout->addWidget(m_stackedWidget);
		setLayout(layout);
	}

	/*! 
	 * @brief 加载本地配置，初始化数据库连接信息
	 */
	void MainWindow::initDatabase()
	{
		ConfigKey ckey;
		DB::DatabaseConfigInfo cinfo;
		QString dtype = Base::RUtil::getGlobalValue(ckey.m_dbGroupId, ckey.m_dbType, "MYSQL").toString();

		if (dtype.toLower().contains("mysql")) {
			cinfo.dbType = DB::DB_MYSQL;
		}
		else {
			cinfo.dbType = DB::DB_ORACLE;
		}

		cinfo.hostName = Base::RUtil::getGlobalValue(ckey.m_dbGroupId, ckey.m_dbHost, "127.0.0.1").toString();
		cinfo.dbName = Base::RUtil::getGlobalValue(ckey.m_dbGroupId, ckey.m_dbName, "DatabaseManage").toString();
		cinfo.dbUser = Base::RUtil::getGlobalValue(ckey.m_dbGroupId, ckey.m_userName, "root").toString();
		cinfo.port = Base::RUtil::getGlobalValue(ckey.m_dbGroupId, ckey.m_dbPort, "3306").toInt();

		bool userInnerPass = Base::RUtil::getGlobalValue(ckey.m_dbGroupId, ckey.m_userInnerPassword, true).toBool();

		if (userInnerPass) {
			cinfo.dbPass = "root";
		}
		else {
			cinfo.dbPass = Base::RUtil::getGlobalValue(ckey.m_dbGroupId, ckey.m_password, "root").toString();
		}
	
		Base::DatabaseManager::instance()->setDatabaseInfo(cinfo);
	}

	void MainWindow::initThread()
	{
		RSingleton<RequestProcessThread>::instance()->startMe();
		connect(RSingleton<RequestProcessThread>::instance(),SIGNAL(sendProcessResult(ResponseUnit *)),this,SLOT(processResponse(ResponseUnit *)));
	}

	void MainWindow::initNetwork()
	{
		ConfigKey ckey;
		QString localDataIp = Base::RUtil::getGlobalValue(ckey.m_netGroupId, ckey.m_remoteServerIp, "127.0.0.1").toString();
		ushort localDataPort = Base::RUtil::getGlobalValue(ckey.m_netGroupId, ckey.m_remoteServerDataPort, 9999).toInt();	
		
		NetAcceptor::instance()->start(localDataIp, localDataPort);
	}

	void MainWindow::processResponse(ResponseUnit * unit)
	{
		NetAcceptor::instance()->processResponseUnit(unit);
	}

} //namespace Related 
