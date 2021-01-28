#include "taskoverviewpage.h"

#include <QDebug>

namespace Related {

	TaskOverViewPage::TaskOverViewPage(QWidget *parent)
		: AbstractPage(parent),
		m_firstLoadData(true)
	{
		init();
		initConnect();
	}

	TaskOverViewPage::~TaskOverViewPage()
	{
	}

	Related::PageType TaskOverViewPage::getPageType() const
	{
		return Page_TaskOverviewPage;
	}

	void TaskOverViewPage::prepareBringToTop()
	{
		if (m_firstLoadData ) {
			refreshCurrTaskSimple();
			m_firstLoadData = false;
		}
	}

	void TaskOverViewPage::setTaskId(QString taskId) 
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

	void TaskOverViewPage::processTaskSimpleResponse(const Datastruct::TaskSimpleResponse & response)
	{
		if (response.m_result == true) {
			m_taskBaseInfoPage->setTaskBaseInfo(response.taskInfo);
		}
	}

	void TaskOverViewPage::init()
	{
		CustomWidgetContainer * taskStatisticsInfoContainer = new CustomWidgetContainer();
		{
			QSize maxSize(415, 170);
			QSize minSize(345, 170);
			m_taskPlatformType = new OverViewItem();
			m_taskPlatformType->setLabelData(QString::number(12));
			m_taskPlatformType->setLabelText(QStringLiteral("平台类型"));
			m_taskPlatformType->setLabelBackground(QColor(237, 168, 27));
			m_taskPlatformType->setLabelIcon(QStringLiteral(":/QYBlue/resource/qyblue/上传.png"));
			m_taskPlatformType->setFixedSize(maxSize);
			m_taskPlatformType->setMinimumSize(minSize);

			m_platformdataSize = new OverViewItem();
			m_platformdataSize->setLabelData("120GB");
			m_platformdataSize->setLabelBackground(QColor(77, 174, 116));
			m_platformdataSize->setLabelText(QStringLiteral("平台占用空间"));
			m_platformdataSize->setLabelIcon(QStringLiteral(":/QYBlue/resource/qyblue/数据库记录.png"));
			m_platformdataSize->setMaximumSize(maxSize);
			m_platformdataSize->setMinimumSize(minSize);

			m_advSize = new OverViewItem();
			m_advSize->setLabelData(QString::number(8));
			m_advSize->setLabelBackground(QColor(199, 99, 116));
			m_advSize->setLabelText(QStringLiteral("音频数据占用空间"));
			m_advSize->setLabelIcon(QStringLiteral(":/QYBlue/resource/qyblue/未处理.png"));
			m_advSize->setMaximumSize(maxSize);
			m_advSize->setMinimumSize(minSize);

			QHBoxLayout * overViewLayout = new QHBoxLayout();
			overViewLayout->setContentsMargins(10, 15, 10, 15);
			overViewLayout->addWidget(m_taskPlatformType);
			overViewLayout->addWidget(m_platformdataSize);
			overViewLayout->addWidget(m_advSize);

			QWidget * tmpWidget = new QWidget();
			tmpWidget->setLayout(overViewLayout);
			taskStatisticsInfoContainer->setFixedHeight(maxSize.height() + overViewLayout->contentsMargins().top() + overViewLayout->contentsMargins().bottom());
			taskStatisticsInfoContainer->setContent(tmpWidget);
		}

		//[] 任务详细信息窗口		
		CustomWidgetContainer * taskBaseInfoContainer = new CustomWidgetContainer();
		{
			m_taskBaseInfoPage = new TaskBaseInfoPage();
			m_taskBaseInfoPage->setMinimumHeight(200);
			m_taskBaseInfoPage->setMaximumHeight(240);

			QHBoxLayout *taskBaseInfoLayout = new QHBoxLayout();
			taskBaseInfoLayout->addWidget(m_taskBaseInfoPage);
			taskBaseInfoLayout->setContentsMargins(0, 0, 0, 0);
			taskBaseInfoContainer->setLayout(taskBaseInfoLayout);
		}
	
		//[]任务结果信息展示窗口
		CustomWidgetContainer * taskResultInfoContainer = new CustomWidgetContainer();
		{
			m_taskResultInfoPage = new TaskResultInfoPage();

			QHBoxLayout *taskResultInfoLayout = new QHBoxLayout();
			taskResultInfoLayout->addWidget(m_taskResultInfoPage);
			taskResultInfoLayout->setContentsMargins(0, 0, 0, 0);
			taskResultInfoContainer->setLayout(taskResultInfoLayout);
		}

		QWidget *mainwidget = new QWidget();
		QVBoxLayout *VLayout = new  QVBoxLayout();
		VLayout->addWidget(taskStatisticsInfoContainer);
		VLayout->addWidget(taskBaseInfoContainer);
		VLayout->addWidget(taskResultInfoContainer);
		VLayout->setContentsMargins(0, 40, 0, 0);
		mainwidget->setLayout(VLayout);
		
		QHBoxLayout *mainLayout = new QHBoxLayout();
		mainLayout->addWidget(mainwidget);
		mainLayout->setContentsMargins(4, 4, 4, 4);
		this->setLayout(mainLayout);
	}

	void TaskOverViewPage::initConnect()
	{
		connect(SignalDispatch::instance(), SIGNAL(respTaskSimpleResponse(const Datastruct::TaskSimpleResponse &)),
			this, SLOT(processTaskSimpleResponse(const Datastruct::TaskSimpleResponse &)));

	}

	void TaskOverViewPage::refreshCurrTaskSimple()
	{
		Datastruct::TaskSimpleRequest request;
		request.taskId = m_taskId;
		NetConnector::instance()->write(request);
	}

} //namespace Related