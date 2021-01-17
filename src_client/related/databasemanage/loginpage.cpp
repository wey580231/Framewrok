#include "loginpage.h"

#include <QHBoxLayout>

namespace Related {

	LoginPage::LoginPage(QWidget *parent)
		: QWidget(parent)
	{
		init();
	}

	LoginPage::~LoginPage()
	{
	}

	void LoginPage::init()
	{
		QWidget * mainWidget = new QWidget();
		mainWidget->setObjectName("Widget_LoginBackGround");

		QWidget * loginWidget = new QWidget();
		


		QHBoxLayout * layout = new QHBoxLayout();
		layout->setContentsMargins(0, 0, 0, 0);
		layout->addWidget(mainWidget);
		setLayout(layout);
	}

} //namespace Related 