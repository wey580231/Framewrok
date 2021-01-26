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
		AbstractPage * cpage = dynamic_cast<AbstractPage *>(m_tabWidget->currentWidget());
		if (cpage)
			cpage->prepareBringToTop();
	}

	void SystemSettingWidget::init()
	{
		m_tabWidget = new Base::RTabWidget();
		m_tabWidget->setTabBarHeight(60);

		m_userManagePage = new  UserManagePage();
		m_otherSettingPage = new  OtherSettingPage();

		m_tabWidget->addPage(QStringLiteral("用户管理"), m_userManagePage);
		m_tabWidget->addPage(QStringLiteral("系统设置"), m_otherSettingPage);

		m_tabWidget->setWidgetBringToBottomCallback([&](QWidget * preWidet) {
			AbstractPage * preparePage = dynamic_cast<AbstractPage *>(preWidet);
			if (preparePage)
				preparePage->prepareBringToBottom();
		});

		m_tabWidget->setWidgetBringToTopCallback([&](QWidget * newWidet) {
			AbstractPage * nextPage = dynamic_cast<AbstractPage *>(newWidet);
			if (nextPage)
				nextPage->prepareBringToTop();
		});

		QVBoxLayout * layout = new QVBoxLayout();
		layout->addWidget(m_tabWidget);
		layout->setContentsMargins(0, 0, 0, 0);
		setLayout(layout);
	}

} //namespace Related 