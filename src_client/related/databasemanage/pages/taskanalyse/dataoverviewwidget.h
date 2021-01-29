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

#include "../abstractpage.h"
#include "../../customwidget/operationtoolspage.h"
#include "../../customwidget/pageswitchbar.h"
#include "tablemodel/dataoverviewmolel.h"

namespace Related {

	class DataOverviewWidget : public AbstractPage
	{
		Q_OBJECT

	public:
		DataOverviewWidget(QWidget *parent = nullptr);
		~DataOverviewWidget();

		PageType getPageType() const;
		void prepareBringToTop();

		void setTaskId(QString taskId);

	private slots:
		void respToolButtPressed(OperationToolsPage::ButtType type);
		void setPageNum(int page);
		void setFixedPageRowCount(int pageItemCount);

	private:
		void init();
		void initConnect();

	private:
		OperationToolsPage *m_operationToolsPage;
		Base::RTableView * m_tableView;
		DataOverviewMolel *m_tableModel;
		PageSwitchBar * m_pageSwitch;
	};

}//namespace Related 