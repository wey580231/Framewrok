#include "widgetcontainer.h"

#include "qboxlayout.h"

namespace Related {

	WidgetContainer::WidgetContainer(QWidget *parent)
		: QWidget(parent)
	{
		init();
		initPages();
	}

	WidgetContainer::~WidgetContainer()
	{
	}

	void WidgetContainer::switchPage(int pageIndex)
	{
		PageType pt = static_cast<PageType>(pageIndex);
		if (m_pageMapping.contains(pt)) {
			m_stackedWidget->setCurrentWidget(m_pageMapping.value(pt));
		}
	}

	void WidgetContainer::init()
	{
		m_stackedWidget = new QStackedWidget();
		m_stackedWidget->setObjectName("Widget_RightPanel");

		QHBoxLayout * layout = new QHBoxLayout();
		layout->setContentsMargins(0, 0, 0, 0);
		layout->addWidget(m_stackedWidget);
		setLayout(layout);
	}

	void WidgetContainer::initPages()
	{
		//主页
		m_mainPage = new MainPage();
		m_stackedWidget->addWidget(m_mainPage);

		//数据管理
		m_dataMangePage = new DataManageWidget();
		m_stackedWidget->addWidget(m_dataMangePage);

		//系统设置
		m_systemSettingPage = new SystemSettingWidget();
		m_stackedWidget->addWidget(m_systemSettingPage);
		
		m_pageMapping.insert(m_mainPage->getPageType(), m_mainPage);
		m_pageMapping.insert(m_dataMangePage->getPageType(), m_dataMangePage);
		m_pageMapping.insert(m_systemSettingPage->getPageType(), m_systemSettingPage);
	}

} //namespace Related 