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

	class TargetOverViewPage;
	class TargetDetailPage;

	class TargetDatabaseWidget : public AbstractPage
	{
		Q_OBJECT

	public:
		TargetDatabaseWidget(QWidget *parent = nullptr);
		~TargetDatabaseWidget();

		PageType getPageType() const;

		void prepareBringToTop();

	private:
		void init();

	private:
		Base::RTabWidget * m_tabWidget;
		TargetOverViewPage * m_targetOverViewPage;				/*!< 目标库概览界面 */
		TargetDetailPage * m_targetDetailPage;					/*!< 目标详情页面 */
	};

}//namespace Related 