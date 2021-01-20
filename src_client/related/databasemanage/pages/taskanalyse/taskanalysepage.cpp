#include "taskanalysepage.h"

namespace Related {

	TaskAnalysePage::TaskAnalysePage(QWidget *parent)
		: AbstractPage(parent)
	{
		init();
	}

	TaskAnalysePage::~TaskAnalysePage()
	{
	}

	Related::PageType TaskAnalysePage::getPageType() const
	{
		return Page_TaskDataAnalyse;
	}

	void TaskAnalysePage::init()
	{

	}

} //namespace Related 