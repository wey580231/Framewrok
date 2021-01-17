#include "systemsettingwidget.h"

namespace Related {

	SystemSettingWidget::SystemSettingWidget(QWidget *parent)
		: AbstractPage(parent)
	{
		init();
	}

	SystemSettingWidget::~SystemSettingWidget()
	{
	}

	Related::PageType SystemSettingWidget::getPageType() const
	{
		return Page_Setting;
	}

	void SystemSettingWidget::respTabChanged(int page)
	{
		m_stackedWidget->setCurrentIndex(page);
	}

	void SystemSettingWidget::init()
	{
		m_tabWidget = new Base::RTabBar();
		connect(m_tabWidget, SIGNAL(currentIndexChanged(int)), this, SLOT(respTabChanged(int)));

		m_tabWidget->setFixedHeight(60);
		m_tabWidget->setTabAlignment(Base::RTabBar::AlignLeft);

		Base::RTabButton * userMangeButt = new Base::RTabButton(QStringLiteral("用户管理"));
		Base::RTabButton * systemSettingButt = new Base::RTabButton(QStringLiteral("系统设置"));

		m_tabWidget->addTabButton(userMangeButt, Tab_userManage);
		m_tabWidget->addTabButton(systemSettingButt, Tab_systemSetting);
		m_tabWidget->setFixedHeight(60);
		m_stackedWidget = new QStackedWidget();

		m_userManagePage = new  UserManagePage();
		m_otherSettingPage = new  OtherSettingPage();

		m_stackedWidget->addWidget(m_userManagePage);
		m_stackedWidget->addWidget(m_otherSettingPage);

		QVBoxLayout * layout = new QVBoxLayout();
		layout->addWidget(m_tabWidget);
		layout->addWidget(m_stackedWidget);
		layout->setContentsMargins(0, 0, 0, 0);
		setLayout(layout);
	}

} //namespace Related 