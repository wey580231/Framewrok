#include "taskrecordpage.h"

#include <QDebug>

namespace Related {

	TaskRecordPage::TaskRecordPage(QWidget *parent)
		: AbstractPage(parent)
	{
		init();
	}

	TaskRecordPage::~TaskRecordPage()
	{
	}

	Related::PageType TaskRecordPage::getPageType() const
	{
		return Page_TaskRecordPage;
	}

	void TaskRecordPage::prepareBringToTop()
	{
		AbstractPage * cpage = dynamic_cast<AbstractPage *>(m_tabWidget->currentWidget());
		if (cpage)
			cpage->prepareBringToTop();
	}

	void TaskRecordPage::setTaskId(QString taskId)
	{
		m_logbookPage->setTaskId(taskId);
		m_trialSheetWidget->setTaskId(taskId);
		m_taskReportsPage->setTaskId(taskId);
	}

	void TaskRecordPage::init()
	{
		m_tabWidget = new Base::RTabWidget();
		m_tabWidget->setTabBarHeight(60);

		m_logbookPage = new DutyRecordPage();

		m_trialSheetWidget = new TrialSheetWidget();

		m_taskReportsPage = new TaskReportPage();

		m_tabWidget->addPage(QStringLiteral("值班日志表"), m_logbookPage);
		m_tabWidget->addPage(QStringLiteral("试验记录表"), m_trialSheetWidget);
		m_tabWidget->addPage(QStringLiteral("任务报告"), m_taskReportsPage);

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

		QVBoxLayout * mainLayout = new QVBoxLayout();
		mainLayout->addWidget(m_tabWidget);
		mainLayout->setContentsMargins(0, 0, 0, 0);
		setLayout(mainLayout);
	}

} //namespace Related 