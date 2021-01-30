/*!
 * @brief     音频数据信息展示界面  
 * @details
 * @author    yzg
 * @version   1.0
 * @date      2021.01.23 14:24:08
 * @warning
 * @copyright NanJing RenGu.
 * @note
 */
#pragma once

#include <QWidget>
#include <QStackedWidget>
#include <QLabel>

#include <base\selfwidget\tablemode\rtableview.h>
#include <base\selfwidget\iconbutton.h>
#include <base/selfwidget/rmessagebox.h>

#include "../abstractpage.h"
#include "../../customwidget/operationtoolspage.h"
#include "tablemodel/audiodatamodel.h"

namespace Related {

	class WavDataPage : public AbstractPage
	{
		Q_OBJECT

	public:
		WavDataPage(QWidget *parent);
		~WavDataPage();

		PageType getPageType() const;

		void prepareBringToTop();

		void setTaskId(QString taskId);

	private slots:
		void slotTableDoubleClicked(QModelIndex index);
		void respButtCliecked();
	private:
		void init();

	private:
		QStackedWidget *m_StackedWidget;

		//[]
		Base::RTableView * m_tableView;
		AudioDataModel *m_tableModel;
		OperationToolsPage *m_operationToolsPage;
		//[]

	};

}//namespace Related 