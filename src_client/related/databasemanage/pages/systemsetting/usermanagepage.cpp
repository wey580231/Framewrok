#include "usermanagepage.h"

#include <QHBoxLayout>
#include <QDebug>

#include <commondefines/protocol.h>
#include "../utils/util.h"
#include "../customwidget/pageswitchbar.h"
#include "../customwidget/customwidgetcontainer.h"
#include "../net/netconnector.h"
#include "../net/signaldispatch.h"
#include "../global.h"

namespace Related {

	UserManagePage::UserManagePage(QWidget *parent)
		: AbstractPage(parent), m_firstLoadData(true)
	{
		init();

		connect(SignalDispatch::instance(), SIGNAL(respQueryUserListResponse(const Datastruct::LoadAllUserResponse &)), 
			this, SLOT(respQueryUserListResponse(const Datastruct::LoadAllUserResponse &)));
	}

	UserManagePage::~UserManagePage()
	{
	}

	PageType UserManagePage::getPageType() const
	{
		return Page_Setting_UserManage;
	}

	void UserManagePage::prepareBringToTop()
	{
		if (m_firstLoadData) {
			refreshCurrPage();
			m_firstLoadData = false;
		}
	}

	void UserManagePage::setPageNum(int page)
	{
		refreshCurrPage();
	}

	/*! 
	 * @brief 处理用户列表结果查询更新
	 * @param response 用户列表查询结果
	 */
	void UserManagePage::respQueryUserListResponse(const Datastruct::LoadAllUserResponse & response)
	{
		m_tableModel->updateData(response.m_userInfos);
		m_pageSwitch->setDataSize(response.m_userCount);
	}

	void UserManagePage::respToolButtPressed(OperationToolsPage::ButtType type)
	{
		switch (type)
		{
			case OperationToolsPage::Butt_Add: {
			}
				break;
			case OperationToolsPage::Butt_Delete: {
			
			}
				break;
			case OperationToolsPage::Butt_Edit: {
			
			}
				break;
			case OperationToolsPage::Butt_Refresh: {
				refreshCurrPage();
			}
				break;
			default:
				break;
		}
	}

	void UserManagePage::setFixedPageRowCount(int pageItemCount)
	{
		m_tableModel->setFixedPageRowCount(pageItemCount);
	}

	void UserManagePage::init()
	{
		m_operationToolsPage = new OperationToolsPage();
		connect(m_operationToolsPage, SIGNAL(buttPressed(OperationToolsPage::ButtType)), this, SLOT(respToolButtPressed(OperationToolsPage::ButtType)));

		m_tableView = new Base::RTableView();
		m_tableView->setFocusPolicy(Qt::NoFocus);
		m_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
		m_tableView->setSelectionMode(QAbstractItemView::SingleSelection);

		m_tableModel = new UserManageModel();

		m_tableView->setModel(m_tableModel);

		m_tableView->addColumnItem(Base::ColumnItem(U_Id, QStringLiteral("序号")));
		m_tableView->addColumnItem(Base::ColumnItem(U_UserName, QStringLiteral("用户名"),140));
		m_tableView->addColumnItem(Base::ColumnItem(U_RegisterTime, QStringLiteral("注册时间"),180));
		m_tableView->addColumnItem(Base::ColumnItem(U_LastLoginTime, QStringLiteral("上次登录时间"),180));
		m_tableView->addColumnItem(Base::ColumnItem(U_UserRights, QStringLiteral("权限")));

		m_pageSwitch = new PageSwitchBar();
		connect(m_pageSwitch, SIGNAL(perPageNumsChanged(int)), this, SLOT(setFixedPageRowCount(int)));
		connect(m_pageSwitch, SIGNAL(switchPage(int)), this, SLOT(setPageNum(int)));

		CustomWidgetContainer * cwidget = new CustomWidgetContainer();
		cwidget->setContent(m_operationToolsPage);

		QWidget * twidget = new QWidget();
		QVBoxLayout * cvlayout = new QVBoxLayout();
		cvlayout->setContentsMargins(0, 0, 0, 0);
		cvlayout->addWidget(m_tableView);
		cvlayout->addWidget(m_pageSwitch);
		twidget->setLayout(cvlayout);

		CustomWidgetContainer * ctableView = new CustomWidgetContainer();
		ctableView->setContent(twidget);

		QVBoxLayout * vlayout = new QVBoxLayout();
		vlayout->setContentsMargins(4, 4, 4, 4);
		vlayout->addWidget(cwidget);
		vlayout->addWidget(ctableView);
		setLayout(vlayout);
	}

	/*! 
	 * @brief 刷新当前页面数据
	 * @details 页面切换、显示条数切换等改变时，调用此方法可获得及时刷新
	 */
	void UserManagePage::refreshCurrPage()
	{
		Datastruct::LoadAllUserRequest request;
		request.m_name = Global::G_UserEntity.name;
		request.m_offsetIndex = m_pageSwitch->dataOffset();
		request.m_limitIndex = m_pageSwitch->perPageCount();
		NetConnector::instance()->write(request);
	}

}//namespace Related 