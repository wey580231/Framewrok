/*!
 * @brief     ֵ����־��  
 * @author    yzg
 * @version   1.0
 * @date      2021.01.20 19:39:23
 * @warning
 * @copyright NanJing RenGu.
 * @note
 */
 
#pragma once

#include <QWidget>
#include <QComboBox>

#include <base\selfwidget\tablemode\rtableview.h>
#include <base\selfwidget\iconbutton.h>

#include "customwidget/operationtoolspage.h"
#include "tablemodel/logbookmodel.h"


namespace Related {

	class LogbookPage : public QWidget
	{
		Q_OBJECT

	public:
		LogbookPage(QWidget *parent = nullptr);
		~LogbookPage();


	private:
		void init();

	private:
		Base::RTableView * m_tableView;
		LogbookModel *m_tableModel;
		OperationToolsPage *m_operationToolsPage;
	};

}//namespace Related
