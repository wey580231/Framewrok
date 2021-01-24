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

	void NewTaskInfoSetWidget::init()
	{
		m_taskPictureUploadWidget = new TaskPictureUploadWidget();
		m_taskPictureUploadWidget->setMinimumSize(150, 120);
		QHBoxLayout * hLayout = new QHBoxLayout();
		hLayout->addStretch();
		hLayout->addWidget(m_taskPictureUploadWidget);
		hLayout->setContentsMargins(0, 0, 0, 0);
		hLayout->addStretch();
		ui.widget->setLayout(hLayout);
	}

}//namespace Related