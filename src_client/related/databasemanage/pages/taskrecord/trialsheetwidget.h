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
#include <QHBoxLayout>

#include <base\selfwidget\tablemode\rtableview.h>
#include <base\selfwidget\iconbutton.h>
#include <base\util\rutil.h>
#include <commondefines/protocol.h>

#include "../abstractpage.h"
#include "../customwidget/pageswitchbar.h"
#include "customwidget/operationtoolspage.h"
#include "tablemodel/trialsheetmodel.h"

namespace Related {

	class TrialSheetWidget : public AbstractPage
	{
		Q_OBJECT

	public:
		TrialSheetWidget(QWidget *parent  = nullptr);
		~TrialSheetWidget();

		PageType getPageType() const;

		void prepareBringToTop();

		void setTaskId(QString taskId);

	private slots:
		void respToolButtPressed(OperationToolsPage::ButtType type);
		void processExperimentRecordCreateResponse(const Datastruct::ExperimentRecordCreateResponse & response);
		void processQueryAllExperimentRecordResponse(const Datastruct::LoadAllExperimentRecordsResponse & response);
		void processExperimentRecordDeleteResponse(const Datastruct::ExperimentRecordDeleteResponse & response);

	private:
		void init();
		void initConnent();

		void insertExperimentRecord();
		void refreshCurrPage();

	private:
		OperationToolsPage * m_operationToolsPage;			/*!< 操作工具页面 */
		Base::RTableView * m_tableView;						/*!< 数据表格 */	
		TrialSheetModel * m_tableModel;						/*!< 数据模型 */
		PageSwitchBar * m_pageSwitch;						/*!< 切换页 */

		bool m_firstLoadData;								/*!< 第一次加载页面显示 */
		QString m_taskId;									/*!< 任务Id */
	};

} // namespace Related 