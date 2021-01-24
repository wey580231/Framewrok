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

#include <commondefines/protocol.h>
#include <base\selfwidget\tablemode\rtableview.h>
#include <base\selfwidget\iconbutton.h>

#include "tablemodel/usermanagemodel.h"
#include "customwidget/operationtoolspage.h"
#include "../abstractpage.h"

namespace Related {

	class PageSwitchBar;

	class UserManagePage : public AbstractPage
	{
		Q_OBJECT

	public:
		UserManagePage(QWidget *parent = nullptr);
		~UserManagePage();

		PageType getPageType() const;

		void prepareBringToTop();

	private slots:
		void setPageNum(int page);
		void respQueryUserListResponse(const Datastruct::LoadAllUserResponse & response);
		void respToolButtPressed(OperationToolsPage::ButtType type);

	private:
		void init();

		void refreshCurrPage();

	private:
		Base::RTableView * m_tableView;
		UserManageModel * m_tableModel;

		OperationToolsPage *m_operationToolsPage;

		PageSwitchBar * m_pageSwitch;		/*!< 页面切换栏 */

		bool m_firstLoadData;	/*!< 第一次加载页面显示 */
	};

}//namespace Related 