/*!
 * @brief     试验记录界面
 * @author    yzg
 * @version   1.0
 * @date      2021.01.23 11:06:46
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
#include "tablemodel/trialsheetmodel.h"

namespace Related {

	class TrialSheetWidget : public QWidget
	{
		Q_OBJECT

	public:
		TrialSheetWidget(QWidget *parent  = nullptr);
		~TrialSheetWidget();


	private:
		void init();

	private:
		Base::RTableView * m_tableView;				
		TrialSheetModel * m_tableModel;
		OperationToolsPage * m_operationToolsPage;
	};

} // namespace Related 