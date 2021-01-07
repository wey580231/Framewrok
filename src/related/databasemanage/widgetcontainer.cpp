#include "widgetcontainer.h"

#include "qboxlayout.h"

namespace Related {

	WidgetContainer::WidgetContainer(QWidget *parent)
		: QWidget(parent)
	{
		init();
	}

	WidgetContainer::~WidgetContainer()
	{
	}

	void WidgetContainer::init()
	{
		m_stackedWidget = new QStackedWidget();
		//m_stackedWidget->setObjectName("Widget_WidgetContainer");
		

		QHBoxLayout * layout = new QHBoxLayout();
		layout->setContentsMargins(0, 0, 0, 0);
		layout->addWidget(m_stackedWidget);
		setLayout(layout);
	}

} //namespace Related 