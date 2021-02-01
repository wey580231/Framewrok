/*!
 * @brief     AISÊý¾Ý¿â 
 * @details 
 * @author    yzg
 * @version   1.0
 * @date      2021.01.31 20:01:08
 * @warning
 * @copyright NanJing RenGu.
 * @note
 */
#pragma once

#include <QWidget>
#include <QStackedWidget>

#include <base\selfwidget\rtabwidget.h>

#include "../abstractpage.h"
#include "aisdatapage.h"

namespace Related {

	class AisDataBasePage :  public AbstractPage
	{
		Q_OBJECT

	public:
		AisDataBasePage(QWidget *parent = nullptr);
		~AisDataBasePage();

		PageType getPageType() const;

		void prepareBringToTop();

	private:
		void init();

	private:
		Base::RTabWidget * m_tabWidget;
		AisDataPage * m_aisDataPage;
	};

}//namespace Related 