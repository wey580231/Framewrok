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
		QWidget * mainWidget = new QWidget();

		m_leftPanel = new LeftPanel(mainWidget);

		m_widgetContainer = new WidgetContainer(mainWidget);

		connect(m_leftPanel, SIGNAL(currentIndexChanged(int)), m_widgetContainer,SLOT(switchPage(int)));

		QHBoxLayout * mainLayout = new QHBoxLayout();
		mainLayout->setSpacing(0);
		mainLayout->setContentsMargins(0, 0, 0, 0);
		mainLayout->addWidget(m_leftPanel);
		mainLayout->addWidget(m_widgetContainer);
		mainWidget->setLayout(mainLayout);
		
		QHBoxLayout * layout = new QHBoxLayout();
		layout->setContentsMargins(0, 0, 0, 0);
		layout->addWidget(mainWidget);
		setLayout(layout);
	}

} //namespace Related 
