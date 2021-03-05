#include "newtaskinfosetwidget.h"

#include <QDebug>
#include <QDateTime>


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

	void NewTaskInfoSetWidget::setTaskBaseInfo(TaskBaseInfo info)
	{
		ui.lineEdit_name->setText(info.taskName);
		ui.lineEdit_local->setText(info.taskLocation);
		ui.lineEdit_local_2->setText(info.lon);
		ui.lineEdit_local_3->setText(info.lat);
	
	}

	TaskBaseInfo NewTaskInfoSetWidget::getTaskBaseInfo()
	{
		m_taskBaseInfo.taskName = ui.lineEdit_name->text();
		m_taskBaseInfo.taskLocation = ui.lineEdit_local->text();
		m_taskBaseInfo.lon = ui.lineEdit_local_2->text();
		m_taskBaseInfo.lat = ui.lineEdit_local_3->text();

		m_taskBaseInfo.startTime = ui.dateTimeEdit_startTime->dateTime().toString("yyyy-MM-dd hh:mm:ss");
		m_taskBaseInfo.endTime = ui.dateTimeEdit_endTime->dateTime().toString("yyyy-MM-dd hh:mm:ss");
		return m_taskBaseInfo;
	}

	void NewTaskInfoSetWidget::init()
	{
		QDateTime dateTime = QDateTime::currentDateTime();
		ui.dateTimeEdit_startTime->setDisplayFormat("yyyy-MM-dd hh:mm:ss");
		ui.dateTimeEdit_startTime->setDateTime(dateTime);
		ui.dateTimeEdit_endTime->setDisplayFormat("yyyy-MM-dd hh:mm:ss");
		ui.dateTimeEdit_endTime->setDateTime(dateTime);

	}

}//namespace Related