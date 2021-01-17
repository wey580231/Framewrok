#include "usermanagepage.h"

#include <QHBoxLayout>
#include <QDebug>

#include "../util.h"
#include "../customwidget/pageswitchbar.h"
#include "../customwidget/customwidgetcontainer.h"

namespace Related {

	UserManagePage::UserManagePage(QWidget *parent)
		: QWidget(parent)
	{
		init();
	}

	UserManagePage::~UserManagePage()
	{
	}

	void UserManagePage::init()
	{
		m_operationToolsPage = new OperationToolsPage();

		m_tableView = new Base::RTableView();
		m_tableView->setFocusPolicy(Qt::NoFocus);
		m_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
		m_tableView->setSelectionMode(QAbstractItemView::SingleSelection);

		m_tableModel = new UserManageModel();
		m_tableModel->prepareData();

		m_tableView->setModel(m_tableModel);

		m_tableView->addColumnItem(Base::ColumnItem(U_Id, QStringLiteral("序号")));
		m_tableView->addColumnItem(Base::ColumnItem(U_UserName, QStringLiteral("用户名"),140));
		m_tableView->addColumnItem(Base::ColumnItem(U_RegisterTime, QStringLiteral("注册时间"),180));
		m_tableView->addColumnItem(Base::ColumnItem(U_LastLoginTime, QStringLiteral("上次登录时间"),180));
		m_tableView->addColumnItem(Base::ColumnItem(U_UserRights, QStringLiteral("权限")));

		PageSwitchBar * pageSwitch = new PageSwitchBar();
		pageSwitch->setDataSize(m_tableModel->datasSize());
		connect(pageSwitch, SIGNAL(perPageNumsChanged(int)), m_tableModel, SLOT(setFixedPageRowCount(int)));
		connect(pageSwitch, SIGNAL(switchPage(int)), this, SLOT(setPageNum(int)));

		CustomWidgetContainer * cwidget = new CustomWidgetContainer();
		cwidget->setContent(m_operationToolsPage);

		QWidget * twidget = new QWidget();
		QVBoxLayout * cvlayout = new QVBoxLayout();
		cvlayout->setContentsMargins(0, 0, 0, 0);
		cvlayout->addWidget(m_tableView);
		cvlayout->addWidget(pageSwitch);
		twidget->setLayout(cvlayout);

		CustomWidgetContainer * ctableView = new CustomWidgetContainer();
		ctableView->setContent(twidget);

		QVBoxLayout * vlayout = new QVBoxLayout();
		vlayout->setContentsMargins(4, 4, 4, 4);
		vlayout->addWidget(cwidget);
		vlayout->addWidget(ctableView);
		setLayout(vlayout);
	}

}//namespace Related 