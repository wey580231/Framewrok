#include "mainwindow.h"

#include <qboxlayout.h>

#include "leftpanel.h"
#include "widgetcontainer.h"

namespace Related {

	MainWindow::MainWindow(QWidget *parent)
		: QWidget(parent)
	{
		init();
	}

	MainWindow::~MainWindow()
	{
	}

	void MainWindow::init()
	{
		m_stackedWidget = new QStackedWidget();

		//µÇÂ¼Ò³Ãæ
		m_loginPage = new LoginPage();
		connect(m_loginPage, SIGNAL(switchToMainPage()), this, SLOT(respLoginSuccess()));

		m_mainWidget = new QWidget();

		m_leftPanel = new LeftPanel(m_mainWidget);
		m_widgetContainer = new WidgetContainer(m_mainWidget);

		connect(m_leftPanel, SIGNAL(currentIndexChanged(int)), m_widgetContainer,SLOT(switchPage(int)));

		QHBoxLayout * mainLayout = new QHBoxLayout();
		mainLayout->setSpacing(0);
		mainLayout->setContentsMargins(0, 0, 0, 0);
		mainLayout->addWidget(m_leftPanel);
		mainLayout->addWidget(m_widgetContainer);
		m_mainWidget->setLayout(mainLayout);

		m_stackedWidget->addWidget(m_loginPage);
		m_stackedWidget->addWidget(m_mainWidget);
		
		QHBoxLayout * layout = new QHBoxLayout();
		layout->setContentsMargins(0, 0, 0, 0);
		layout->addWidget(m_stackedWidget);
		setLayout(layout);
	}

	void MainWindow::respLoginSuccess()
	{
		m_stackedWidget->setCurrentWidget(m_mainWidget);
	}

} //namespace Related 
