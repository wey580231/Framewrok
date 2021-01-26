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
		return m_taskBaseInfo;
	}

	void NewTaskInfoSetWidget::init()
	{

	}

}//namespace Related