#pragma once

#include <QWidget>

namespace Related {

	class LeftPanel;
	class TitlePanel;
	class WidgetContainer;

	class MainWindow : public QWidget
	{
		Q_OBJECT

	public:
		MainWindow(QWidget *parent = nullptr);
		~MainWindow();

	private:
		void init();

	private:
		LeftPanel * m_leftPanel;
		TitlePanel * m_titlePanel;
		WidgetContainer * m_widgetContainer;
	};

}	//namespace Related 