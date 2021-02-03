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

#include <base\selfwidget\rtabwidget.h>
#include "../abstractpage.h"

namespace Related {

	class TargetDatabaseManagePage;
	class TargetDetailPage;

	class TargetDatabaseWidget : public AbstractPage
	{
		Q_OBJECT

	public:
		TargetDatabaseWidget(QWidget *parent = nullptr);
		~TargetDatabaseWidget();

		PageType getPageType() const;

	private:
		void init();

	private:
		Base::RTabWidget * m_tabWidget;

		TargetDatabaseManagePage * m_targetDatabaseManagePage;
		TargetDetailPage * m_targetDetailPage;
	};

}//namespace Related 