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

			m_taskBaseInfo.taskName = response.taskInfo.taskName;				
			m_taskBaseInfo.taskLocation = response.taskInfo.location;			
			m_taskBaseInfo.startTime = response.taskInfo.startTime;				
			m_taskBaseInfo.endTime = response.taskInfo.endTime;				
			m_taskBaseInfo.lon = response.taskInfo.lon;					
			m_taskBaseInfo.lat = response.taskInfo.lat;					
			m_taskBaseInfo.taskDescription = response.taskInfo.description;		
		}
	}

	void TaskOverViewPage::processQueryAllTaskDetectPlatformResponse(const Datastruct::LoadAllTaskDetectPlatformResponse & response)
	{
		qDebug() << "_____________________processQueryAllTaskDetectPlatformResponse________________________";
	}

	void TaskOverViewPage::processQueryAllTaskImageResponse(const Datastruct::LoadAllTaskImageResponse & response)
	{
		if (m_imagesItems.size() > 0) {
			std::for_each(m_imagesItems.begin(), m_imagesItems.end(), [](TestImagesItem * item) {
				delete item;
			});
			m_imagesItems.clear();
		}
	
		m_taskImageInfos = response.m_taskImageInfos;

		for (int i = 0; i < response.m_taskImageCount; i++) {
			TestImagesItem *item = new TestImagesItem();
			item->setImagesType(TestImagesItem::TI_Sketch);
			item->setImagesIndex(i + 1);
			item->setMinimumSize(280, 270);
			item->setMaximumSize(300, 290);
			connect(item, SIGNAL(signalSeleteImagesIndex(int)), this, SLOT(slotSelectedImagesIndex(int)));
			m_imagesItems.append(item);
		}
		updateTaskImages();
	}

	void TaskOverViewPage::slotSelectedImagesIndex(int index)
	{
		TestImagesDetailDialog dialog(this);
		dialog.exec();
	}

	void TaskOverViewPage::slotModiftTaskBaseInfo()
	{
		NewTaskDialog dialog(m_taskId, NewTaskDialog::Task_Modify, this);
		dialog.setTaskBaseInfo(m_taskBaseInfo);
		dialog.setTaskImages(m_taskImageInfos);

		if (QDialog::Accepted == dialog.exec()) {
			refreshCurrTaskSimple();
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
			m_taskPlatformType->setLabelIcon(WRAP_RESOURCE(上传));
			m_taskPlatformType->setFixedSize(maxSize);
			m_taskPlatformType->setMinimumSize(minSize);

			m_platformdataSize = new OverViewItem();
			m_platformdataSize->setLabelData("120GB");
			m_platformdataSize->setLabelBackground(QColor(77, 174, 116));
			m_platformdataSize->setLabelText(QStringLiteral("平台占用空间"));
			m_platformdataSize->setLabelIcon(WRAP_RESOURCE(数据库记录));
			m_platformdataSize->setMaximumSize(maxSize);
			m_platformdataSize->setMinimumSize(minSize);

			m_advSize = new OverViewItem();
			m_advSize->setLabelData(QString::number(8));
			m_advSize->setLabelBackground(QColor(199, 99, 116));
			m_advSize->setLabelText(QStringLiteral("音频数据占用空间"));
			m_advSize->setLabelIcon(WRAP_RESOURCE(未处理));
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
			m_modifyTaskButt = new Base::RIconButton();
			m_modifyTaskButt->setText(QStringLiteral("修改任务"));
			m_modifyTaskButt->setMinimumSize(60, 30);
			m_modifyTaskButt->setIcon(QIcon(WRAP_RESOURCE(新增)));
			connect(m_modifyTaskButt, SIGNAL(clicked()), this, SLOT(slotModiftTaskBaseInfo()));

			QWidget * btnWidget = new QWidget();
			QHBoxLayout * btnLayout = new QHBoxLayout();
			btnLayout->addWidget(m_modifyTaskButt);
			btnLayout->addStretch();
			btnLayout->setContentsMargins(4,4,4,4);
			btnWidget->setLayout(btnLayout);

			m_taskBaseInfoPage = new TaskBaseInfoPage();
			m_taskBaseInfoPage->setMinimumHeight(200);
			m_taskBaseInfoPage->setMaximumHeight(240);

			QVBoxLayout *taskBaseInfoLayout = new QVBoxLayout();
			taskBaseInfoLayout->addWidget(btnWidget);
			taskBaseInfoLayout->addWidget(m_taskBaseInfoPage);
			taskBaseInfoLayout->setContentsMargins(0, 0, 0, 0);
			taskBaseInfoContainer->setLayout(taskBaseInfoLayout);
		}
	
		//[]任务结果信息展示窗口
		CustomWidgetContainer * taskResultInfoContainer = new CustomWidgetContainer();
		{
			m_testImagesScrollArea = new QScrollArea();
			m_testImagesScrollArea->setStyleSheet("background-color:rgba(0,0,0,0)");
			m_testImagesScrollArea->setWidgetResizable(true);

			m_testImagesWidget = new QWidget();
			m_testImagesWidget->setStyleSheet("background-color:rgba(0,0,0,0)");
			m_testImagesScrollArea->setWidget(m_testImagesWidget);

			QHBoxLayout *taskResultInfoLayout = new QHBoxLayout();
			taskResultInfoLayout->addWidget(m_testImagesScrollArea);
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

		connect(SignalDispatch::instance(), SIGNAL(respQueryAllTaskDetectPlatformResponse(const Datastruct::LoadAllTaskDetectPlatformResponse &)),
			this, SLOT(processQueryAllTaskDetectPlatformResponse(const Datastruct::LoadAllTaskDetectPlatformResponse &)));

		connect(SignalDispatch::instance(), SIGNAL(respQueryAllTaskImageResponse(const Datastruct::LoadAllTaskImageResponse &)),
			this, SLOT(processQueryAllTaskImageResponse(const Datastruct::LoadAllTaskImageResponse &)));
	}

	void TaskOverViewPage::updateTaskImages()
	{
		QGridLayout * gLayout = nullptr;

		if (m_testImagesWidget->layout() == nullptr) {
			gLayout = new QGridLayout();
			gLayout->setContentsMargins(0, 0, 0, 0);
			m_testImagesWidget->setLayout(gLayout);
		}
		else {
			gLayout = dynamic_cast<QGridLayout *>(m_testImagesWidget->layout());
			for (int i = gLayout->count() - 1; i >= 0; i--) {
				if (gLayout->itemAt(i)->widget()) {
					delete gLayout->takeAt(i);
				}
			}
		}

		for (int i = 0; i < m_imagesItems.size(); i++) {
			int row = i / 5;
			int column = i % 5;
			gLayout->addWidget(m_imagesItems.at(i), row, column, 1, 1);
		}
	}

	void TaskOverViewPage::refreshCurrTaskSimple()
	{
		// 任务基本信息
		Datastruct::TaskSimpleRequest request;
		request.taskId = m_taskId;
		DataNetConnector::instance()->write(request);

		// 任务侦测平台
		Datastruct::LoadAllTaskDetectPlatformRequest taskDetectPlatformRequest;
		taskDetectPlatformRequest.m_taskId = m_taskId;
		DataNetConnector::instance()->write(taskDetectPlatformRequest);

		// 任务试验图片
		Datastruct::LoadAllTaskImageRequest taskImageRequest;
		taskImageRequest.m_taskId = m_taskId;
		DataNetConnector::instance()->write(taskImageRequest);
	}

} //namespace Related