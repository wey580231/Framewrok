#include "mainwindow.h"

#include <QHBoxLayout>
#include <QDebug>

#include <base/util/rutil.h>
#include <base/util/rsingleton.h>

#include "../net/requestprocessthread.h"
#include "../net/netacceptor.h"

namespace Related {

	MainWindow::MainWindow(QWidget *parent)
		: QWidget(parent)
	{
		init();
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
