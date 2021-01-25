#include "othersettingpage.h"

#include <QHBoxLayout>

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

	void OtherSettingPage::init()
	{

	}

} //namespace Related 