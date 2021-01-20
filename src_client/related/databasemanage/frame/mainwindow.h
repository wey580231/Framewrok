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

#include "loginpage.h"

#include "pages\systemmainpage\systemmainpage.h"
#include "pages\databasemanage\datamanagewidget.h"
#include "pages\systemsetting\systemsettingwidget.h"

#include "pages\taskoverview\taskoverviewpage.h"
#include "pages\taskrecord\taskrecordpage.h"
#include "pages\taskanalyse\taskanalysepage.h"

#include "pages/targetdatabase/targetdatabasewidget.h"

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
		void initConnect();

	private slots:
		void respLoginSuccess();
		void switchPage(int pageIndex);
		void siwtchToTaskView(QString taskId);
		void switchToSystemView();

	private:
		QStackedWidget * m_stackedWidget;
		LeftPanel * m_leftPanel;

		LoginPage * m_loginPage;
		
		QStackedWidget * m_pageContainer;
		QMap<PageType, AbstractPage *> m_pageMapping;

		SystemMainPage * m_mainPage;
		DataManageWidget * m_dataMangePage;
		SystemSettingWidget * m_systemSettingPage;

		TaskOverViewPage * m_taskOverViewPage;
		TaskRecordPage * m_taskRecordPage;
		TaskAnalysePage * m_taskAnalysePage;

		TargetDatabaseWidget *m_targetDatabaseWidget;

		QWidget * m_mainWidget;
	};

}	//namespace Related 