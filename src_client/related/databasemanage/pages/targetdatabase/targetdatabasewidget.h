/*!
 * @brief     目标库界面  
 * @details   提供
 * @author    yzg
 * @version   1.0
 * @date      2021.01.20 10:43:37
 * @warning
 * @copyright NanJing RenGu.
 * @note
 */
 
#pragma once

#include <QWidget>
#include <qstackedwidget.h>

#include <base\selfwidget\rtabbar.h>
#include "../abstractpage.h"

#include "targetdatabasemanagepage.h"

namespace Related {

	class TargetDatabaseWidget : public AbstractPage
	{
		Q_OBJECT

	public:
		TargetDatabaseWidget(QWidget *parent = nullptr);
		~TargetDatabaseWidget();

		PageType getPageType() const;

		enum TargerTabBarIndex {
			Tab_target1,
			Tab_target2,
		};

	private slots:
		void respTabChanged(int page);

	private:
		void init();

	private:
		Base::RTabBar * m_tabWidget;
		QStackedWidget * m_stackedWidget;

		TargetDatabaseManagePage *m_targetDatabaseManagePage;
	};

}//namespace Related 