#include "mainwindow.h"

#include <qboxlayout.h>

#include "titlepanel.h"
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
		QWidget * mainWidget = new QWidget();

		m_leftPanel = new LeftPanel(mainWidget);

		m_titlePanel = new TitlePanel(mainWidget);
		m_widgetContainer = new WidgetContainer(mainWidget);

		QVBoxLayout * rightLayout = new QVBoxLayout();
		rightLayout->addWidget(m_titlePanel);
		rightLayout->addWidget(m_widgetContainer);

		QHBoxLayout * mainLayout = new QHBoxLayout();
		mainLayout->setContentsMargins(0, 0, 0, 0);
		
		mainLayout->addWidget(m_leftPanel);
		mainLayout->addLayout(rightLayout);

		mainWidget->setLayout(mainLayout);
		
		QHBoxLayout * layout = new QHBoxLayout();
		layout->setContentsMargins(0, 0, 0, 0);
		layout->addWidget(mainWidget);
		setLayout(layout);
	}

} //namespace Related 
