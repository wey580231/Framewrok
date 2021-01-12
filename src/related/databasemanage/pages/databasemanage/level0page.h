/*!
 * @brief     0级库数据管理页面   
 * @author    wey
 * @version   1.0
 * @date      2021.01.11 11:04:20
 * @warning
 * @copyright NanJing RenGu.
 * @note
 */
 
#pragma once

#include <QWidget>
#include <QComboBox>

#include <base\selfwidget\tablemode\rtableview.h>
#include <base\selfwidget\iconbutton.h>

#include "tablemodel/levelmodel0.h"

namespace Related {

	class Level0Page : public QWidget
	{
		Q_OBJECT

	public:
		Level0Page(QWidget *parent);
		~Level0Page();

	private:
		void init();

	private:
		Base::RTableView * m_tableView;
		LevelModel0 * m_tableModel;

		Base::RIconButton * m_addButton;
		Base::RIconButton * m_delButton;
		Base::RIconButton * m_editButton;
		Base::RIconButton * m_refreshButton;

		Base::RIconButton * m_searchButton;
		Base::RIconButton * m_advanceSearchButton;
		Base::RIconButton * m_moreButton;

		Base::RIconButton * m_firstButton;
		Base::RIconButton * m_prePageButton;
		Base::RIconButton * m_nextPageButton;
		Base::RIconButton * m_lastButton;

		QComboBox * m_pageItemCountBox;
	};

} //namespace Related 