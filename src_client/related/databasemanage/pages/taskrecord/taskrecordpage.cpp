#include "taskrecordpage.h"

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

	void TaskRecordPage::slotRespTabChanged(int page)
	{
		m_stackedWidget->setCurrentIndex(page);
	}

	void TaskRecordPage::init()
	{
		m_tabWidget = new Base::RTabBar();
		connect(m_tabWidget, SIGNAL(currentIndexChanged(int)), this, SLOT(slotRespTabChanged(int)));

		m_tabWidget->setFixedHeight(60);
		m_tabWidget->setTabAlignment(Base::RTabBar::AlignLeft);


		Base::RTabButton * logbookButt = new Base::RTabButton(QStringLiteral("值班日志表"));
		Base::RTabButton * trialSheetButt = new Base::RTabButton(QStringLiteral("试验记录表"));
		Base::RTabButton * taskReportsButt = new Base::RTabButton(QStringLiteral("任务报告"));

		m_tabWidget->addTabButton(logbookButt, Tab_logbook);
		m_tabWidget->addTabButton(trialSheetButt, Tab_trialSheet);
		m_tabWidget->addTabButton(taskReportsButt, Tab_taskReports);

		m_tabWidget->setFixedHeight(60);

		m_stackedWidget = new QStackedWidget();

		m_logbookPage = new LogbookPage();

		m_trialSheetWidget = new TrialSheetWidget();

		m_taskReportsWidget = new TaskReportsWidget();

		m_stackedWidget->addWidget(m_logbookPage);
		m_stackedWidget->addWidget(m_trialSheetWidget);
		m_stackedWidget->addWidget(m_taskReportsWidget);


		QVBoxLayout * mainLayout = new QVBoxLayout();
		mainLayout->addWidget(m_tabWidget);
		mainLayout->addWidget(m_stackedWidget);
		mainLayout->setContentsMargins(0, 0, 0, 0);
		setLayout(mainLayout);
	}

} //namespace Related 