/*!
 * @brief     值班日志表  
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
#include <QHBoxLayout>

#include <base\selfwidget\tablemode\rtableview.h>
#include <base\selfwidget\iconbutton.h>
#include <base\util\rutil.h>
#include <commondefines/protocol.h>

#include "../abstractpage.h"
#include "../customwidget/pageswitchbar.h"

#include "customwidget/operationtoolspage.h"
#include "tablemodel/logbookmodel.h"

namespace Related {

	class DutyRecordPage : public AbstractPage
	{
		Q_OBJECT

	public:
		DutyRecordPage(QWidget *parent = nullptr);
		~DutyRecordPage();

		PageType getPageType() const;

		void prepareBringToTop();

		void setTaskId(QString taskId);

	private slots:
		void respToolButtPressed(OperationToolsPage::ButtType type);
		void processDutyRecordCreateResponse(const Datastruct::DutyRecordCreateResponse & response);
		void processQueryAllDutyRecordResponse(const Datastruct::LoadAllDutyRecordResponse & response);
		void processDutyRecordDeleteResponse(const Datastruct::DutyRecordDeleteResponse & response);

	private:
		void init();
		void initConnect();
		void insertDutyRecord();

		void refreshCurrPage();

	private:
		OperationToolsPage *m_operationToolsPage;			/*!< 操作工具页面 */
		Base::RTableView * m_tableView;						/*!< 数据表格 */
		LogbookModel * m_tableModel;						/*!< 数据模型 */
		PageSwitchBar * m_pageSwitch;						/*!< 切换页 */				

		bool m_firstLoadData;								/*!< 第一次加载页面显示 */
		QString m_taskId;									/*!< 任务Id */
	};

}//namespace Related
