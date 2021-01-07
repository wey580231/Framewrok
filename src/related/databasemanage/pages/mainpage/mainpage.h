/*!
 *  @brief     数据概览显示页面
 *  @details   显示当前数据库概要信息，通过图标或区块集中展示。
 *  @author    wey
 *  @version   1.0
 *  @date      2021.01.07
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#pragma once

#include <QWidget>

#include "overviewitem.h"
#include "../abstractpage.h"

namespace Related {

	class MainPage : public AbstractPage
	{
		Q_OBJECT

	public:
		MainPage(QWidget *parent = nullptr);
		~MainPage();

		PageType getPageType() const;

	private:
		void init();

	private:
		OverViewItem * m_todayAddItem;
		OverViewItem * m_totalItem;
		OverViewItem * m_unprocessItem;
	};

} //namespace Related