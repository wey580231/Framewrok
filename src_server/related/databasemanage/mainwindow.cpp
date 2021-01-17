#include "mainwindow.h"

#include <qboxlayout.h>

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

		QWidget * mainWidget = new QWidget();

		m_stackedWidget->addWidget(mainWidget);
		
		QHBoxLayout * layout = new QHBoxLayout();
		layout->setContentsMargins(0, 0, 0, 0);
		layout->addWidget(m_stackedWidget);
		setLayout(layout);
	}

} //namespace Related 
