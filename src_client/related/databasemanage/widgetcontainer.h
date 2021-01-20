/*!
 *  @brief     主页面窗口区
 *  @details   以容器方式，堆叠多个数据页面，通过左侧的列表可进行切换。
 *  @author    wey
 *  @version   1.0
 *  @date      2021.01.07
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#pragma once

#include <QWidget>
#include <qstackedwidget.h>
#include <qmap.h>

#include "pages\mainpage\mainpage.h"
#include "pages\databasemanage\datamanagewidget.h"
#include "pages\systemsetting\systemsettingwidget.h"

namespace Related {

	class WidgetContainer : public QWidget
	{
		Q_OBJECT

	public:
		WidgetContainer(QWidget *parent);
		~WidgetContainer();

	private slots:
		void switchPage(int pageIndex);

	private:
		void init();
		void initPages();

	private:
		QStackedWidget * m_stackedWidget;

		MainPage * m_mainPage;
		DataManageWidget * m_dataMangePage;
		SystemSettingWidget * m_systemSettingPage;

		QMap<PageType, AbstractPage *> m_pageMapping;
	};
} //namespace Related 