#include "taskoverviewpage.h"

#include <QHBoxLayout>

namespace Related {

	TaskOverViewPage::TaskOverViewPage(QWidget *parent)
		: AbstractPage(parent)
	{
		init();
	}

	TaskOverViewPage::~TaskOverViewPage()
	{
	}

	Related::PageType TaskOverViewPage::getPageType() const
	{
		return Page_TaskOverviewPage;
	}

	void TaskOverViewPage::init()
	{

	}

} //namespace Related