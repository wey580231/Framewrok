#pragma once

#include <QWidget>
#include <qstackedwidget.h>

namespace Related {

	class WidgetContainer : public QWidget
	{
		Q_OBJECT

	public:
		WidgetContainer(QWidget *parent);
		~WidgetContainer();

	private:
		void init();

	private:
		QStackedWidget * m_stackedWidget;

	};
} //namespace Related 