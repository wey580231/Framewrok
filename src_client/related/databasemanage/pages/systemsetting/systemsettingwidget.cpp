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

	/*!
	 * @details  1.触发加载查询用户信息(只页面第一次被显示时调用)
	 */
	void SystemSettingWidget::prepareBringToTop()
	{
		AbstractPage * cpage = dynamic_cast<AbstractPage *>(m_stackedWidget->currentWidget());
		if (cpage)
			cpage->prepareBringToTop();
	}

	void SystemSettingWidget::respTabChanged(int page)
	{
		AbstractPage * preparePage = dynamic_cast<AbstractPage *>(m_stackedWidget->currentWidget());
		if (preparePage)
			preparePage->prepareBringToBottom();

		m_stackedWidget->setCurrentIndex(page);

		AbstractPage * nextPage = dynamic_cast<AbstractPage *>(m_stackedWidget->currentWidget());
		if (nextPage)
			nextPage->prepareBringToTop();
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