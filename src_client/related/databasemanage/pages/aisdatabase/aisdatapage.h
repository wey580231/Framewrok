/*!
 * @brief     AIS数据展示页面
 * @details 
 * @author    yzg
 * @version   1.0
 * @date      2021.01.31 20:15:48
 * @warning
 * @copyright NanJing RenGu.
 * @note
 */
 #pragma once

#include <QWidget>
#include <QStackedWidget>
#include <QHBoxLayout>

#include <base\selfwidget\rtabwidget.h>
#include <base\selfwidget\tablemode\rtableview.h>
#include <base\selfwidget\iconbutton.h>
#include <base/selfwidget/rmessagebox.h>
#include <base\util\rutil.h>
#include <commondefines/protocol.h>

#include "../abstractpage.h"
#include "../../customwidget/pageswitchbar.h"
#include "../../customwidget/operationtoolspage.h"

#include "tabelmodel/aisdatamodel.h"

namespace Related {

	class AisDataPage : public AbstractPage
	{
		Q_OBJECT

	public:
		AisDataPage(QWidget *parent = nullptr);
		~AisDataPage();

		PageType getPageType() const;
		void prepareBringToTop();

	private slots:
		void respToolButtPressed(OperationToolsPage::ButtType type);
		void setPageNum(int page);
		void setFixedPageRowCount(int pageItemCount);

		void processQueryAllAISDataResponse(const Datastruct::LoadAllAISDatasResponse & response);
		void processAISDataCreateResponse(const Datastruct::AISDataCreateResponse & response);
		void processAISDataDeleteResponse(const Datastruct::AISDataDeleteResponse & response);
		void processAISDataModifyResponse(const Datastruct::AISDataModifyResponse & response);
	private:
		void init();
		void initConnect();
		void refreshCurrPage();

		void createAISData();

	private:
		OperationToolsPage * m_operationToolsPage;				/*!< 操作工具页面 */
		Base::RTableView * m_tableView;							/*!< 数据表格 */
		AisDataModel * m_tableModel;							/*!< 数据模型 */
		PageSwitchBar * m_pageSwitch;							/*!< 切换页 */

		bool m_firstLoadData;									/*!< 第一次加载页面显示 */
	};

}// namespace Related 
