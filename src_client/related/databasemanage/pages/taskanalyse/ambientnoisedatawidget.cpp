#include "ambientnoisedatawidget.h"

#include <QDebug>


#include "../../utils/util.h"
#include "../../customwidget/customwidgetcontainer.h"

namespace Related {

	AmbientNoiseDataWidget::AmbientNoiseDataWidget(QWidget *parent)
		: AbstractPage(parent)
	{
		init();
	}

	AmbientNoiseDataWidget::~AmbientNoiseDataWidget()
	{
	}

	PageType AmbientNoiseDataWidget::getPageType() const
	{
		return PageType();
	}

	void AmbientNoiseDataWidget::prepareBringToTop()
	{
	}

	void AmbientNoiseDataWidget::setTaskId(QString taskId)
	{
	}

	void AmbientNoiseDataWidget::respToolButtPressed(OperationToolsPage::ButtType type)
	{

	}

	void AmbientNoiseDataWidget::setPageNum(int page)
	{

	}

	void AmbientNoiseDataWidget::setFixedPageRowCount(int pageItemCount)
	{
	}

	void AmbientNoiseDataWidget::init()
	{
		m_operationToolsPage = new OperationToolsPage();

		m_tableView = new Base::RTableView();
		m_tableView->setFocusPolicy(Qt::NoFocus);
		m_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
		m_tableView->setSelectionMode(QAbstractItemView::SingleSelection);

		m_tableModel = new AmbientNoiseModel();
		m_tableModel->prepareData();

		m_tableView->setModel(m_tableModel);

		m_tableView->addColumnItem(Base::ColumnItem(T_Index, QStringLiteral("索引")));
		m_tableView->addColumnItem(Base::ColumnItem(T_TargetName, QStringLiteral("文件名称"), 140));
		m_tableView->addColumnItem(Base::ColumnItem(T_Edttime, QStringLiteral("平台名称"), 180));
		m_tableView->addColumnItem(Base::ColumnItem(T_Tonnage, QStringLiteral("时长"), 180));
		m_tableView->addColumnItem(Base::ColumnItem(T_AxlesNumber, QStringLiteral("数据大小")));
		m_tableView->addColumnItem(Base::ColumnItem(T_Datalength, QStringLiteral("起始索引")));
		m_tableView->addColumnItem(Base::ColumnItem(T_Type, QStringLiteral("结束索引")));

		m_pageSwitch = new PageSwitchBar();
		m_pageSwitch->setDataSize(m_tableModel->datasSize());
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

	void AmbientNoiseDataWidget::initConnect()
	{
	}

}//namespace Related 