#include "newtaskinfosetwidget.h"

namespace Related {

	NewTaskInfoSetWidget::NewTaskInfoSetWidget(QWidget *parent)
		: QWidget(parent)
	{
		ui.setupUi(this);
		init();
	}

	NewTaskInfoSetWidget::~NewTaskInfoSetWidget()
	{
	}

	/*!
	* @brief  获取任务的基本信息
	* @details 
	*/
	TaskBaseInfo NewTaskInfoSetWidget::getTaskBaseInfo()
	{
		m_taskBaseInfo.id			= Base::RUtil::UUID();
		m_taskBaseInfo.taskName		= ui.lineEdit_name->text();
		m_taskBaseInfo.taskplace	= ui.lineEdit_local->text();
		m_taskBaseInfo.user			= ui.lineEdit_user->text();
		//m_taskBaseInfo.startTime	= ui.dateTimeEdit_startTime->toString("yyyyMMddhhmmss");
		//m_taskBaseInfo.startTime	= ui.dateTimeEdit_endTime->toString("yyyy-MM-d hh:mm:ss");

		return m_taskBaseInfo;
	}

	void  NewTaskInfoSetWidget::slotPictureUploadPath(QString path)
	{
		m_taskBaseInfo.taskPicture = path;
	}

	void NewTaskInfoSetWidget::init()
	{
		m_taskPictureUploadWidget = new TaskPictureUploadWidget();
		m_taskPictureUploadWidget->setMinimumSize(150, 120);
		connect(m_taskPictureUploadWidget, SIGNAL(signalPictureUploadPath(QString)),
			this, SLOT(slotPictureUploadPath(QString)));

		QHBoxLayout * hLayout = new QHBoxLayout();
		hLayout->addStretch();
		hLayout->addWidget(m_taskPictureUploadWidget);
		hLayout->setContentsMargins(0, 0, 0, 0);
		hLayout->addStretch();
		ui.widget->setLayout(hLayout);
	}

}//namespace Related