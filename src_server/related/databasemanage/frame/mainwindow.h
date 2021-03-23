/*!
 * @brief     数据管理主页面窗口   
 * @author    wey
 * @version   1.0
 * @date      2021.01.16 17:29:08
 * @warning
 * @copyright NanJing RenGu.
 * @note
 */
 #pragma once

#include <QWidget>
#include <QStackedWidget>
#include <QMap>

#include "../datastruct.h"
#include "../global.h"

namespace Related {

	class LeftPanel;
	class WidgetContainer;

	class MainWindow : public QWidget
	{
		Q_OBJECT

	public:
		MainWindow(QWidget *parent = nullptr);
		~MainWindow();

	private:
		void init();
		void initDatabase();
		void initThread();
		void initNetwork();

	private slots:
		void processResponse(ResponseUnit * unit);

	private:
		QStackedWidget * m_stackedWidget;
	};

}	//namespace Related 