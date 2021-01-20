#include "targetdatabasemanagepage.h"

#include <QHBoxLayout>
#include <QDebug>

#include "../utils/util.h"
#include "../customwidget/pageswitchbar.h"
#include "../customwidget/customwidgetcontainer.h"

namespace Related {

	TargetDatabaseManagePage::TargetDatabaseManagePage(QWidget *parent)
		: QWidget(parent)
	{
		init();
	}

	TargetDatabaseManagePage::~TargetDatabaseManagePage()
	{
	}

	void TargetDatabaseManagePage::init()
	{
		m_operationToolsPage = new OperationToolsPage();

		m_tableView = new Base::RTableView();
		m_tableView->setFocusPolicy(Qt::NoFocus);
		m_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
		m_tableView->setSelectionMode(QAbstractItemView::SingleSelection);

		m_tableModel = new TargetDatabaseManageModel();
		m_tableModel->prepareData();

		m_tableView->setModel(m_tableModel);

		m_tableView->addColumnItem(Base::ColumnItem(T_Index, QStringLiteral("索引")));
		m_tableView->addColumnItem(Base::ColumnItem(T_TargetName, QStringLiteral("目标名称"), 140));
		m_tableView->addColumnItem(Base::ColumnItem(T_Edttime, QStringLiteral("录取时间"), 180));
		m_tableView->addColumnItem(Base::ColumnItem(T_Tonnage, QStringLiteral("吨位"), 180));
		m_tableView->addColumnItem(Base::ColumnItem(T_AxlesNumber, QStringLiteral("轴数")));
		m_tableView->addColumnItem(Base::ColumnItem(T_Datalength, QStringLiteral("数据时长")));
		m_tableView->addColumnItem(Base::ColumnItem(T_Type, QStringLiteral("类型")));

		PageSwitchBar * pageSwitch = new PageSwitchBar();
		pageSwitch->setDataSize(m_tableModel->datasSize());
		//connect(pageSwitch, SIGNAL(perPageNumsChanged(int)), m_tableModel, SLOT(setFixedPageRowCount(int)));
		//connect(pageSwitch, SIGNAL(switchPage(int)), this, SLOT(setPageNum(int)));

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
