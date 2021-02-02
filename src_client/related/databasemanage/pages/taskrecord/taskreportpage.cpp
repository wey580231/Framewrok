#include "taskreportpage.h"

#include <QDebug>

#include "../../customwidget/customwidgetcontainer.h"

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

	void TaskReportPage::slotPreviewReport()
	{
		m_stackedWidget->setCurrentIndex(1);
	}

	void TaskReportPage::slotQuiePreviewReport()
	{
		m_stackedWidget->setCurrentIndex(0);
	}

	void TaskReportPage::slotSaveReport()
	{
	}

	void  TaskReportPage::init()
	{
		QWidget * editReportWidget = new QWidget();
		{
			CustomWidgetContainer * operationToolsWidget = new CustomWidgetContainer();
			{
				m_previewButt = new Base::RIconButton();
				m_previewButt->setText(QStringLiteral("预览"));
				m_previewButt->setMinimumSize(60, 30);
				m_previewButt->setIcon(QIcon(WRAP_RESOURCE(新增)));
				connect(m_previewButt, SIGNAL(clicked()), this, SLOT(slotPreviewReport()));

				m_saveButt = new Base::RIconButton();
				m_saveButt->setText(QStringLiteral("保存"));
				m_saveButt->setMinimumSize(60, 30);
				m_saveButt->setIcon(QIcon(WRAP_RESOURCE(新增)));
				connect(m_previewButt, SIGNAL(clicked()), this, SLOT(slotSaveReport()));

				QWidget *hWidget = new QWidget();
				QHBoxLayout * hLayout = new QHBoxLayout();
				hLayout->addStretch();
				hLayout->addWidget(m_previewButt);
				hLayout->addWidget(m_saveButt);
				hLayout->setContentsMargins(0, 0, 0, 0);
				hWidget->setLayout(hLayout);
				hWidget->setMinimumHeight(40);
				hWidget->setMaximumHeight(40);
				operationToolsWidget->setContent(hWidget);
			}

			CustomWidgetContainer * editWidget = new CustomWidgetContainer();
			{
// 				m_taskReportEditPage = new TaskReportEditPage();
// 				editWidget->setContent(m_taskReportEditPage);
			}

			QVBoxLayout * editLayout = new QVBoxLayout();
			editLayout->addWidget(operationToolsWidget, 1);
			editLayout->addWidget(editWidget, 500);
			editLayout->setContentsMargins(0,0,0,0);
			editReportWidget->setLayout(editLayout);
		}

		// 预览界面
		QWidget * previewReportWidget = new QWidget();
		{
			QWidget *quitWidget = new QWidget();
			{
				m_quitButt = new Base::RIconButton();
				m_quitButt->setText(QStringLiteral("退出"));
				m_quitButt->setMinimumSize(60, 30);
				m_quitButt->setIcon(QIcon(WRAP_RESOURCE(新增)));
				connect(m_quitButt, SIGNAL(clicked()), this, SLOT(slotQuiePreviewReport()));

				QHBoxLayout * hLayout = new QHBoxLayout();
				hLayout->addStretch();
				hLayout->addWidget(m_quitButt);
				hLayout->setContentsMargins(0,0,0,0);
				quitWidget->setLayout(hLayout);
				quitWidget->setMinimumHeight(40);
				quitWidget->setMaximumHeight(40);
			}

			CustomWidgetContainer * previewPWidget = new CustomWidgetContainer();
			{
				m_taskReportPreviewPage = new TaskReportPreviewPage();
				previewPWidget->setContent(m_taskReportPreviewPage);
			}

			QVBoxLayout * vLayout = new QVBoxLayout();
			vLayout->addWidget(quitWidget, 1);
			vLayout->addWidget(previewPWidget, 500);
			vLayout->setContentsMargins(0,0,0,0);
			previewReportWidget->setLayout(vLayout);
		}

		m_stackedWidget = new QStackedWidget();
		m_stackedWidget->addWidget(editReportWidget);
		m_stackedWidget->addWidget(previewReportWidget);

		QVBoxLayout * mainLayout = new QVBoxLayout();
		mainLayout->addWidget(m_stackedWidget);
		mainLayout->setContentsMargins(4,4,4,4);
		this->setLayout(mainLayout);
	}

	void  TaskReportPage::initConnect()
	{

	}

}//namespace Related 