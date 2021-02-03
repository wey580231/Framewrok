#include "targetdetailpage.h"

#include <QLabel>

namespace Related {

	TargetDetailPage::TargetDetailPage(QWidget *parent)
		: AbstractPage(parent)
	{
		init();
	}

	TargetDetailPage::~TargetDetailPage()
	{
	}

	PageType TargetDetailPage::getPageType() const
	{
		return Page_TargetDatabase_TargetDetail;
	}

	void TargetDetailPage::init()
	{

	}

} //namespace Related
