#include "taskrecordpage.h"

namespace Related {

	TaskRecordPage::TaskRecordPage(QWidget *parent)
		: AbstractPage(parent)
	{
	}

	TaskRecordPage::~TaskRecordPage()
	{
	}

	Related::PageType TaskRecordPage::getPageType() const
	{
		return Page_TaskRecordPage;
	}

	void TaskRecordPage::init()
	{

	}

} //namespace Related 