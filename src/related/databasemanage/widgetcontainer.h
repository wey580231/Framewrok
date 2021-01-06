#pragma once

#include <QWidget>

namespace Related {

	class WidgetContainer : public QWidget
	{
		Q_OBJECT

	public:
		WidgetContainer(QWidget *parent);
		~WidgetContainer();
	};
} //namespace Related 