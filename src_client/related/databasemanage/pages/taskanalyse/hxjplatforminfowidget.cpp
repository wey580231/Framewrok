#include "hxjplatforminfowidget.h"

#include <QDebug>
#include <QHBoxLayout>

#include "../../utils/util.h"
#include "../../customwidget/pageswitchbar.h"
#include "../../customwidget/customwidgetcontainer.h"

namespace Related {

	HXJPlatformInfoWidget::HXJPlatformInfoWidget(QWidget *parent)
		: QWidget(parent)
	{
		init();
	}

	HXJPlatformInfoWidget::~HXJPlatformInfoWidget()
	{

	}

	void HXJPlatformInfoWidget::init()
	{
		m_operationToolsPage = new OperationToolsPage();

		m_tableView = new Base::RTableView();
		m_tableView->setFocusPolicy(Qt::NoFocus);
		m_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
		m_tableView->setSelectionMode(QAbstractItemView::SingleSelection);

		m_tableModel = new HXJPlatformInfoModel();
		m_tableModel->prepareData();

		m_tableView->setModel(m_tableModel);

		m_tableView->addColumnItem(Base::ColumnItem(T_Index, QStringLiteral("索引")));
		m_tableView->addColumnItem(Base::ColumnItem(T_TargetName, QStringLiteral("平台名称"), 140));
		m_tableView->addColumnItem(Base::ColumnItem(T_Edttime, QStringLiteral("数据帧索引"), 180));
		m_tableView->addColumnItem(Base::ColumnItem(T_Tonnage, QStringLiteral("录入时间"), 180));
		m_tableView->addColumnItem(Base::ColumnItem(T_AxlesNumber, QStringLiteral("航向角")));
		m_tableView->addColumnItem(Base::ColumnItem(T_Datalength, QStringLiteral("俯仰角")));
		m_tableView->addColumnItem(Base::ColumnItem(T_Type, QStringLiteral("横滚角")));

		PageSwitchBar * pageSwitch = new PageSwitchBar();
		pageSwitch->setDataSize(m_tableModel->datasSize());

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
