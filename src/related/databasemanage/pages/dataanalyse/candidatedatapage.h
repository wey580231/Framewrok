/*!
 *  @brief     候选数据页面
 *  @details   
 *  @author    yzg
 *  @version   1.0
 *  @date      2021.01.14
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#pragma once

#include <QObject>
#include <QWidget>
#include <QComboBox>

#include <base\selfwidget\tablemode\rtableview.h>
#include <base\selfwidget\iconbutton.h>

#include "tablemodel/candidatedatamodel.h"

namespace Related {

	class CandidateDataPage : public QWidget
	{
		Q_OBJECT

	public:
		CandidateDataPage(QWidget *parent = nullptr);
		~CandidateDataPage();

	private:
		void init();

	private:
		Base::RTableView * m_tableView;
		CandidateDataModel * m_tableModel;
	};

}//namespace Related 
