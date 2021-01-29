#include "othersettingpage.h"

#include <QDebug>

#include "../../net/netconnector.h"
#include "../../net/signaldispatch.h"

#include "../../customwidget/customwidgetcontainer.h"

namespace Related {

	OtherSettingPage::OtherSettingPage(QWidget *parent)
		: AbstractPage(parent)
	{
		init();
		initConnect();
	}

	OtherSettingPage::~OtherSettingPage()
	{
	}

	PageType OtherSettingPage::getPageType() const
	{
		return Page_Setting_SystemSetting;
	}


	void OtherSettingPage::processQueryAllDetectPlatformResponse(Datastruct::LoadAllDetectPlatformsResponse response )
	{
	}

	void OtherSettingPage::processQueryAllDetectPlatformSubtypeResponse(Datastruct::LoadAllDetectPlatformSubtypesResponse response)
	{
	}

	void OtherSettingPage::init()
	{

	}

	void OtherSettingPage::initConnect()
	{
		
		CustomWidgetContainer * ctableView = new CustomWidgetContainer();
		{
			m_comboBox = new QComboBox();

		}

	}

	void OtherSettingPage::refreshCurrDetectPlatform()
	{
		Datastruct::LoadAllDetectPlatformsRequest request;
		NetConnector::instance()->write(request);
	}

	void OtherSettingPage::refreshCurrDetectPlatformSubtype(int detectId)
	{
		Datastruct::LoadAllDetectPlatformsRequest request;
		NetConnector::instance()->write(request);
	}

} //namespace Related 