/*!
 *  @brief     数据管理页面
 *  @details   提供对数据库多种数据表显示，提供表操作工具
 *  @author    wey
 *  @version   1.0
 *  @date      2021.01.07
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#pragma once

#include <QWidget>
#include <QStackedWidget>

#include <base\selfwidget\rtabbar.h>

#include "../abstractpage.h"
#include "level0page.h"

namespace Related {

	class DataManageWidget : public AbstractPage
	{
		Q_OBJECT

	public:
		DataManageWidget(QWidget *parent = nullptr);
		~DataManageWidget();

		PageType getPageType() const;

		enum TabBarIndex {
			Tab_level0,
			Tab_level1,
			Tab_level2,
			Tab_other
		};

	private slots:
		void respTabChanged(int page);

	private:
		void init();

	private:
		Base::RTabBar * m_tabWidget;

		Level0Page * m_pageLevel0;

		QStackedWidget * m_stackedWidget;
	};

} //namespace Related 