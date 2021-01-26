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

	TaskBaseInfo NewTaskInfoSetWidget::getTaskBaseInfo()
	{
		m_taskBaseInfo.taskName = ui.lineEdit_name->text();
		m_taskBaseInfo.taskLocation = ui.lineEdit_local->text();
		m_taskBaseInfo.lon = ui.lineEdit_local_2->text().toDouble();
		m_taskBaseInfo.lat = ui.lineEdit_local_3->text().toDouble();

		m_taskBaseInfo.startTime = ui.dateTimeEdit_startTime->dateTime().toString("yyyy.MM.dd hh:mm:ss.zzz");
		m_taskBaseInfo.endTime = ui.dateTimeEdit_endTime->dateTime().toString("yyyy.MM.dd hh:mm:ss.zzz");
	/*	m_taskBaseInfo.taskDescription = ui.plainTextEdit->te*/
		return m_taskBaseInfo;
	}

	void NewTaskInfoSetWidget::init()
	{

	}

}//namespace Related