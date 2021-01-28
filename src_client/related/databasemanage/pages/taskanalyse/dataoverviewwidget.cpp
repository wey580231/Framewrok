#include "dataoverviewwidget.h"

#include <QDebug>

#include "../../utils/util.h"
#include "../../customwidget/pageswitchbar.h"
#include "../../customwidget/customwidgetcontainer.h"

namespace Related {

	DataOverviewWidget::DataOverviewWidget(QWidget *parent)
		: QWidget(parent)
	{
		init();
	}

	DataOverviewWidget::~DataOverviewWidget()
	{

	}

	void DataOverviewWidget::init()
	{
		//
		CustomWidgetContainer * cwidget = new CustomWidgetContainer();
		{
// 			QLabel * t_input = new QLabel(QStringLiteral("录取时间："));
// 			QDateTimeEdit *t_startTime = new QDateTimeEdit();
// 			QLabel * t_input = new QLabel(QStringLiteral("-"));
// 			QDateTimeEdit *t_endTime = new QDateTimeEdit();
// 
// 			QLabel * t_input = new QLabel(QStringLiteral("平台名称："));
// 			QComboBox *t_platformComboBox = new QComboBox();
// 
// 
// 			Base::RIconButton * t_ retrieveButt = new Base::RIconButton();
// 			retrieveButt->setIconSize(Base::RIconButton::ICON_16);
// 			retrieveButt->setText(QStringLiteral("JIA"));
// 			retrieveButt->setFont(QFont(QStringLiteral("微软雅黑"), 10));
// 			butt->setMinimumSize(60, 30);

			m_operationToolsPage = new OperationToolsPage();
			cwidget->setContent(m_operationToolsPage);
		}
	


	

		m_tableView = new Base::RTableView();
		m_tableView->setFocusPolicy(Qt::NoFocus);
		m_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
		m_tableView->setSelectionMode(QAbstractItemView::SingleSelection);

		m_tableModel = new DataOverviewMolel();
		m_tableModel->prepareData();

		m_tableView->setModel(m_tableModel);

		m_tableView->addColumnItem(Base::ColumnItem(T_Index, QStringLiteral("索引")));
		m_tableView->addColumnItem(Base::ColumnItem(T_TargetName, QStringLiteral("编号"), 140));
		m_tableView->addColumnItem(Base::ColumnItem(T_Edttime, QStringLiteral("录入时间"), 180));
		m_tableView->addColumnItem(Base::ColumnItem(T_Tonnage, QStringLiteral("试验名"), 180));
		m_tableView->addColumnItem(Base::ColumnItem(T_AxlesNumber, QStringLiteral("平台名")));
		m_tableView->addColumnItem(Base::ColumnItem(T_Datalength, QStringLiteral("数据时长")));
		m_tableView->addColumnItem(Base::ColumnItem(T_Type, QStringLiteral("类型")));

		PageSwitchBar * pageSwitch = new PageSwitchBar();
		pageSwitch->setDataSize(m_tableModel->datasSize());



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