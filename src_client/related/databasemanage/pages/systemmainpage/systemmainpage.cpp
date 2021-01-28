#include "systemmainpage.h"

#include <QDebug>

#include "../../net/signaldispatch.h"
#include "../../net/netconnector.h"
#include "../../customwidget/customwidgetcontainer.h"

namespace Related {

	SystemMainPage::SystemMainPage(QWidget *parent)
		: AbstractPage(parent),
		m_taskNumItem(nullptr),
		m_diskSpaceItem(nullptr),
		m_platNumItem(nullptr),
		m_newTaskButt(nullptr),
		m_refreshTaskButt(nullptr), 
		m_firstLoadData(true)
	{
		m_taskItems.clear();
		init();
		initConnent();
	}

	SystemMainPage::~SystemMainPage()
	{
	}

	Related::PageType SystemMainPage::getPageType() const
	{
		return Page_SystemMainPage;
	}
	
	/*!
	 * @brief   刷新任务列表信息
	 * @details 
	 */
	void SystemMainPage::prepareBringToTop() {
		if (m_firstLoadData) {
			refreshCurrTask();
			m_firstLoadData = false;
		}
	}

	void SystemMainPage::init()
	{
		QWidget * mainWidget = new QWidget();

		//概览页面
		CustomWidgetContainer * overViewContainer = new CustomWidgetContainer();
		{
			QSize maxSize(415, 170);
			QSize minSize(345, 170);
			m_taskNumItem = new OverViewItem();
			m_taskNumItem->setLabelData(QString::number(0));
			m_taskNumItem->setLabelText(QStringLiteral("任务总数"));
			m_taskNumItem->setLabelBackground(QColor(237, 168, 27));
			m_taskNumItem->setLabelIcon(QStringLiteral(":/QYBlue/resource/qyblue/上传.png"));
			m_taskNumItem->setFixedSize(maxSize);
			m_taskNumItem->setMinimumSize(minSize);

			m_diskSpaceItem = new OverViewItem();
			m_diskSpaceItem->setLabelData(QString("%1 GB").arg(QString::number(0)));
			m_diskSpaceItem->setLabelBackground(QColor(77, 174, 116));
			m_diskSpaceItem->setLabelText(QStringLiteral("占用空间"));
			m_diskSpaceItem->setLabelIcon(QStringLiteral(":/QYBlue/resource/qyblue/数据库记录.png"));
			m_diskSpaceItem->setMaximumSize(maxSize);
			m_diskSpaceItem->setMinimumSize(minSize);

			m_platNumItem = new OverViewItem();
			m_platNumItem->setLabelData(QString::number(0));
			m_platNumItem->setLabelBackground(QColor(199, 99, 116));
			m_platNumItem->setLabelText(QStringLiteral("平台数量"));
			m_platNumItem->setLabelIcon(QStringLiteral(":/QYBlue/resource/qyblue/未处理.png"));
			m_platNumItem->setMaximumSize(maxSize);
			m_platNumItem->setMinimumSize(minSize);

			QHBoxLayout * overViewLayout = new QHBoxLayout();
			overViewLayout->setContentsMargins(10, 15, 10, 15);
			overViewLayout->addWidget(m_taskNumItem);
			overViewLayout->addWidget(m_diskSpaceItem);
			overViewLayout->addWidget(m_platNumItem);

			QWidget * tmpWidget = new QWidget();
			tmpWidget->setLayout(overViewLayout);
			overViewContainer->setFixedHeight(maxSize.height() + overViewLayout->contentsMargins().top() + overViewLayout->contentsMargins().bottom());
			overViewContainer->setContent(tmpWidget);
		}

		//任务概览
		CustomWidgetContainer * taskContainer = new CustomWidgetContainer();
		{
			m_newTaskButt = new Base::RIconButton();
			m_newTaskButt->setText(QStringLiteral("新建任务"));
			m_newTaskButt->setMinimumSize(60,30);
			m_newTaskButt->setIcon(QIcon(QStringLiteral(":/QYBlue/resource/qyblue/新增.png")));		
			connect(m_newTaskButt, SIGNAL(clicked()), this, SLOT(slotNewTaskClickde()));
			m_refreshTaskButt = new Base::RIconButton();
			m_refreshTaskButt->setText(QStringLiteral("刷新任务"));
			m_refreshTaskButt->setMinimumSize(60, 30);
			m_refreshTaskButt->setIcon(QIcon(QStringLiteral(":/QYBlue/resource/qyblue/刷新.png")));
			connect(m_refreshTaskButt, SIGNAL(clicked()), this, SLOT(slotRefreshTaskClicked()));

			m_timeRange = new TimeRangeEdit();

			m_locationBox = new QComboBox();
			m_locationBox->setView(new QListView());
			m_locationBox->addItem(QStringLiteral("选择海区"));

			m_platBox = new QComboBox();
			m_platBox->setView(new QListView());
			m_platBox->addItem(QStringLiteral("选择平台"));

			QHBoxLayout * hlayout = new QHBoxLayout();
			hlayout->setContentsMargins(0, 0, 0, 0);
			hlayout->addWidget(m_newTaskButt);
			hlayout->addWidget(m_refreshTaskButt);
			hlayout->addStretch(1);
			hlayout->addWidget(m_timeRange);
			hlayout->addWidget(m_locationBox);
			hlayout->addWidget(m_platBox);

			m_taskSrollArea = new QScrollArea();
			m_taskSrollArea->setStyleSheet("background-color:rgba(0,0,0,0)");
			m_taskSrollArea->setWidgetResizable(true);

			m_taskWindow = new QWidget();
			m_taskWindow->setStyleSheet("background-color:rgba(0,0,0,0)");
			//m_taskWindow->setObjectName("widgetContainer");
			m_taskSrollArea->setWidget(m_taskWindow);

			QWidget * tmpWidget = new QWidget();
			QVBoxLayout * tmpLayout = new QVBoxLayout();
			tmpLayout->setContentsMargins(0, 0, 0, 0);
			tmpLayout->addLayout(hlayout);
			tmpLayout->addWidget(m_taskSrollArea);
			tmpWidget->setLayout(tmpLayout);

			taskContainer->setContent(tmpWidget);
		}

		QVBoxLayout * mainLayout = new QVBoxLayout();
		mainLayout->setContentsMargins(0, 40, 0, 0);
		mainLayout->addWidget(overViewContainer);
		mainLayout->addWidget(taskContainer);

		mainWidget->setLayout(mainLayout);

		QHBoxLayout * layout = new QHBoxLayout();
		layout->setContentsMargins(4, 4, 4, 4);
		layout->addWidget(mainWidget);
		setLayout(layout);
	}

	void SystemMainPage::initConnent()
	{
		connect(SignalDispatch::instance(), SIGNAL(respTaskCreateResponse(const Datastruct::TaskCreateResponse &)),
			this, SLOT(processTaskCreateResponse(const Datastruct::TaskCreateResponse &)));

		connect(SignalDispatch::instance(), SIGNAL(respQueryAllTaskResponse(const Datastruct::LoadAllTaskResponse &)),
			this, SLOT(processQueryAllTaskResponse(const Datastruct::LoadAllTaskResponse &)));

		connect(SignalDispatch::instance(), SIGNAL(respTaskeDleteResponse(const Datastruct::TaskDeleteResponse &)),
			this, SLOT(processTaskDeleteResponse(const Datastruct::TaskDeleteResponse &)));
	}

	void SystemMainPage::slotNewTaskClickde()
	{
		NewTaskDialog dialog(this);
		dialog.exec();
	}

	void SystemMainPage::slotRefreshTaskClicked()
	{
		refreshCurrTask();
	}

	void SystemMainPage::slotDeleteTask(QString taskId)
	{
		Datastruct::TaskDeleteRequest request;
		request.taskId = taskId;
		NetConnector::instance()->write(request);
	}

	void SystemMainPage::processTaskCreateResponse(const Datastruct::TaskCreateResponse & response)
	{
		if (response.m_createResult == true) {
			refreshCurrTask();
		}
	}

	void SystemMainPage::processQueryAllTaskResponse(const Datastruct::LoadAllTaskResponse & response)
	{
		if (m_taskItems.size() > 0) {
			for (int i = 0; i < m_taskItems.size(); i++) {
				TaskOverViewItem *item = m_taskItems.at(i);
				if (item != nullptr) {
					item->hide();
					delete item;
					item = nullptr;
				}
			}
			m_taskItems.clear();
		}
		if (response.m_taskInfos.size() > 0) {
			m_taskNumItem->setLabelData(QString::number(response.m_taskInfos.size()));

			//[] 刷新显示窗口
			for (int i = 0; i < response.m_taskInfos.size(); i++) {
				Datastruct::TaskEntityData taskData = response.m_taskInfos.at(i);

				TaskOverViewItem * item = new TaskOverViewItem();
				item->setTaskBaseInfo(taskData);
				connect(item, SIGNAL(openTask(QString)), this, SIGNAL(openTask(QString)));
				connect(item, SIGNAL(deleteTask(QString)), this, SLOT(slotDeleteTask(QString)));
				m_taskItems.append(item);
			}
			UpdateTaskListWidget();
		}
	}

	void SystemMainPage::processTaskDeleteResponse(const Datastruct::TaskDeleteResponse & response)
	{
		if (response.m_deleteResult == true) {
			refreshCurrTask();
		}
	}

	/*!
	 * @brief	更新当前任务
	 * @details 任务创建、任删除等改变时，调用此方法可获得及时刷新
	 */
	void SystemMainPage::refreshCurrTask()
	{
		Datastruct::LoadAllTaskRequest request;

		NetConnector::instance()->write(request);
	}

	void SystemMainPage::UpdateTaskListWidget()
	{
		QGridLayout * glayout = nullptr;

		if (m_taskWindow->layout() == nullptr) {
			glayout = new QGridLayout();
			glayout->setContentsMargins(4, 4, 4, 4);
			m_taskWindow->setLayout(glayout);
		}
		else {
			//TDOD 20210126 存在内存泄漏问题
			glayout = dynamic_cast<QGridLayout *>(m_taskWindow->layout());
// 			for (int i = glayout->count(); i >= 0; i--) {
// 				if (glayout->itemAt(i)->widget()) {
// 					delete glayout->takeAt(i);
// 				}
// 			}
			delete m_taskWindow->layout();
			glayout = new QGridLayout();
			glayout->setContentsMargins(4, 4, 4, 4);
			m_taskWindow->setLayout(glayout);
		}

		for (int i = 0; i < m_taskItems.size(); i++) {
			int row = i / 5;
			int column = i % 5;
			glayout->addWidget(m_taskItems.at(i), row, column, 1, 1);
		}
	}

} //namespace Related 