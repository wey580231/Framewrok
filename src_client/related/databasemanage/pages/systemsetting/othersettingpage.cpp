#include "othersettingpage.h"

#include <QDebug>

#include "../../net/datanetconnector.h"
#include "../../net/signaldispatch.h"

#include "../../customwidget/customwidgetcontainer.h"

namespace Related {

	OtherSettingPage::OtherSettingPage(QWidget *parent)
		: AbstractPage(parent)
	{
		init();
	}

	OtherSettingPage::~OtherSettingPage()
	{
	}

	PageType OtherSettingPage::getPageType() const
	{
		return Page_Setting_SystemSetting;
	}

	void OtherSettingPage::prepareBringToTop()
	{
		m_detectPlatformPage->prepareBringToTop();
	}

	void OtherSettingPage::init()
	{
		m_detectPlatformPage = new DetectPlatformPage();
		m_detectPlatformPage->setMaximumHeight(400);
		m_detectPlatformPage->setMaximumWidth(250);

		QVBoxLayout * mainLayout = new QVBoxLayout();
		mainLayout->addWidget(m_detectPlatformPage);
		mainLayout->addStretch();
		setLayout(mainLayout);
	}

} //namespace Related 