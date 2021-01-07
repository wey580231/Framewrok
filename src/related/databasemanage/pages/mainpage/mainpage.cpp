#include "mainpage.h"

#include <qboxlayout.h>
#include <qlabel.h>

namespace Related {

	MainPage::MainPage(QWidget *parent)
		: AbstractPage(parent)
	{
		init();
	}

	MainPage::~MainPage()
	{
	}

	Related::PageType MainPage::getPageType() const
	{
		return Page_MainPage;
	}

	void MainPage::init()
	{
		QWidget * mainWidget = new QWidget();

		QWidget * overViewContainer = new QWidget();
		overViewContainer->setObjectName("widgetContainer");

		QSize maxSize(415,170);
		QSize minSize(345,170);
		m_todayAddItem = new OverViewItem();
		m_todayAddItem->setLabelText(QStringLiteral("今日新增条数"));
		m_todayAddItem->setLabelBackground(QColor(237,168,27));
		m_todayAddItem->setLabelIcon(QStringLiteral(":/QYBlue/resource/qyblue/上传.png"));
		m_todayAddItem->setMaximumSize(maxSize);
		m_todayAddItem->setMinimumSize(minSize);

		m_totalItem = new OverViewItem();
		m_totalItem->setLabelBackground(QColor(77,174,116));
		m_totalItem->setLabelText(QStringLiteral("识别库条数"));
		m_totalItem->setLabelIcon(QStringLiteral(":/QYBlue/resource/qyblue/数据库记录.png"));
		m_totalItem->setMaximumSize(maxSize);
		m_totalItem->setMinimumSize(minSize);

		m_unprocessItem = new OverViewItem();
		m_unprocessItem->setLabelBackground(QColor(199,99,116));
		m_unprocessItem->setLabelText(QStringLiteral("未处理条数"));
		m_unprocessItem->setLabelIcon(QStringLiteral(":/QYBlue/resource/qyblue/未处理.png"));
		m_unprocessItem->setMaximumSize(maxSize);
		m_unprocessItem->setMinimumSize(minSize);

		QHBoxLayout * overViewLayout = new QHBoxLayout();
		overViewLayout->setContentsMargins(10, 15, 10, 15);
		overViewLayout->addWidget(m_todayAddItem);
		overViewLayout->addWidget(m_totalItem);
		overViewLayout->addWidget(m_unprocessItem);

		overViewContainer->setLayout(overViewLayout);

		QVBoxLayout * mainLayout = new QVBoxLayout();
		mainLayout->setContentsMargins(6, 0, 6, 0);
		mainLayout->addWidget(overViewContainer);
		mainLayout->addStretch(1);

		mainWidget->setLayout(mainLayout);

		QHBoxLayout * layout = new QHBoxLayout();
		layout->addWidget(mainWidget);
		setLayout(layout);
	}

} //namespace Related 