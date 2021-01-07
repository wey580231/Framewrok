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

		QWidget * rightWidget = new QWidget();
		rightWidget->setObjectName("Widget_RightPanel");

		QVBoxLayout * rightLayout = new QVBoxLayout();
		rightLayout->setSpacing(0);
		rightLayout->addWidget(m_widgetContainer);
		rightWidget->setLayout(rightLayout);

		QHBoxLayout * mainLayout = new QHBoxLayout();
		mainLayout->setSpacing(0);
		mainLayout->setContentsMargins(0, 0, 0, 0);
		
		mainLayout->addWidget(m_leftPanel);
		mainLayout->addWidget(rightWidget);

		mainWidget->setLayout(mainLayout);
		
		QHBoxLayout * layout = new QHBoxLayout();
		layout->setContentsMargins(0, 0, 0, 0);
		layout->addWidget(mainWidget);
		setLayout(layout);
	}

} //namespace Related 
