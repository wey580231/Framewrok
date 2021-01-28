#include "taskreportpage.h"

#include <QDebug>

namespace Related {

	TaskReportPage::TaskReportPage(QWidget *parent)
		: AbstractPage(parent),
		m_firstLoadData(true)
	{
		init();
		initConnect();
	}

	TaskReportPage::~TaskReportPage()
	{
	}

	Related::PageType TaskReportPage::getPageType() const
	{
		return Page_TaskRecord_TaskReport;
	}

	void TaskReportPage::prepareBringToTop()
	{
		if (m_firstLoadData ) {

			m_firstLoadData = false;
		}
	}

	void TaskReportPage::setTaskId(QString taskId)
	{
		if (m_taskId.isEmpty()) {
			m_taskId = taskId;
		}
		else
		{
			if (m_taskId != taskId) {
				m_taskId = taskId;
				m_firstLoadData = true;
			}
		}
	}

	void  TaskReportPage::init()
	{

	}

	void  TaskReportPage::initConnect()
	{

	}

}//namespace Related 