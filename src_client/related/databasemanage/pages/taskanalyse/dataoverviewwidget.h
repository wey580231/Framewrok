/*!
 * @brief      数据预览界面
 * @details
 * @author    yzg
 * @version   1.0
 * @date      2021.01.23 14:18:49
 * @warning
 * @copyright NanJing RenGu.
 * @note
 */
 
#pragma once
#include <QWidget>
#include <QLabel>
#include <QDateTimeEdit>
#include <QComboBox>
#include <QHBoxLayout>

#include <base\selfwidget\tablemode\rtableview.h>
#include <base\selfwidget\iconbutton.h>

#include "customwidget/operationtoolspage.h"
#include "tablemodel/dataoverviewmolel.h"

namespace Related {

	class DataOverviewWidget : public QWidget
	{
		Q_OBJECT

	public:
		DataOverviewWidget(QWidget *parent);
		~DataOverviewWidget();

	private:
		void init();

	private:
		Base::RTableView * m_tableView;
		DataOverviewMolel *m_tableModel;
		OperationToolsPage *m_operationToolsPage;
	};

}//namespace Related 