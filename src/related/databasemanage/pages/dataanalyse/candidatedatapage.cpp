#include "candidatedatapage.h"

#include <QVBoxLayout>
#include "../customwidget/pageswitchbar.h"

namespace Related {

	CandidateDataPage::CandidateDataPage(QWidget *parent)
		: QWidget(parent)
	{
		init();
	}

	CandidateDataPage::~CandidateDataPage()
	{

	}

	void CandidateDataPage::init()
	{
		m_tableView = new Base::RTableView();
		m_tableView->setFocusPolicy(Qt::NoFocus);
		m_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
		m_tableView->setSelectionMode(QAbstractItemView::SingleSelection);
		m_tableView->addColumnItem(Base::ColumnItem(D_Id, QStringLiteral("序号")));
		m_tableView->addColumnItem(Base::ColumnItem(D_BatchNumber, QStringLiteral("批号")));
		m_tableView->addColumnItem(Base::ColumnItem(D_Time, QStringLiteral("时间")));
		m_tableView->addColumnItem(Base::ColumnItem(D_LocalDistinguish, QStringLiteral("识别结果")));
		m_tableView->addColumnItem(Base::ColumnItem(D_Result, QStringLiteral("置信度")));
		m_tableView->addColumnItem(Base::ColumnItem(D_PlatformName, QStringLiteral("认定状态")));

		m_tableModel = new CandidateDataModel();
		m_tableModel->prepareData();
		m_tableView->setModel(m_tableModel);

		PageSwitchBar * pageSwitch = new PageSwitchBar();
		connect(pageSwitch, SIGNAL(perPageNumsChanged(int)), m_tableModel, SLOT(setFixedPageRowCount(int)));

		QVBoxLayout * tableLayout = new QVBoxLayout();
		tableLayout->addWidget(m_tableView);
		tableLayout->addWidget(pageSwitch);
		tableLayout->setContentsMargins(4, 4, 4, 4);

		setLayout(tableLayout);
	}

}//namespace Related 