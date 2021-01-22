/*!
 *  @brief     用户信息管理页面
 *  @details   
 *  @author    yzg
 *  @version   1.0
 *  @date      2021.01.07
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#pragma once

#include <QWidget>
#include <QComboBox>

#include <base\selfwidget\tablemode\rtableview.h>
#include <base\selfwidget\iconbutton.h>

#include "tablemodel/usermanagemodel.h"
#include "customwidget/operationtoolspage.h"

namespace Related {

	class UserManagePage : public QWidget
	{
		Q_OBJECT

	public:
		UserManagePage(QWidget *parent = nullptr);
		~UserManagePage();

	private slots:
		void setPageNum(int page);

	private:
		void init();

	private:
		Base::RTableView * m_tableView;
		UserManageModel * m_tableModel;

		OperationToolsPage *m_operationToolsPage;
	};

}//namespace Related 