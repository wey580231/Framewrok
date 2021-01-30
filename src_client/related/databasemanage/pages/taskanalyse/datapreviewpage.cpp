#include "datapreviewpage.h"

#include <QDebug>

#include "../../utils/util.h"
#include "../../customwidget/customwidgetcontainer.h"

namespace Related {

	DataPreviewPage::DataPreviewPage(QWidget *parent)
		: AbstractPage(parent),
		m_firstLoadData(false)
	{
		init();
		initConnect();
	}

	DataPreviewPage::~DataPreviewPage()
	{

	}

	PageType DataPreviewPage::getPageType() const
	{
		return Page_TaskAnalyse_DataOverview;
	}

	void DataPreviewPage::prepareBringToTop()
	{
		if (m_firstLoadData) {

			m_firstLoadData = false;
		}
	}

	void DataPreviewPage::setTaskId(QString taskId)
	{
	}

	void DataPreviewPage::respToolButtPressed(OperationToolsPage::ButtType type)
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

		}
			break;
		default:
			break;
		}
	}

	void DataPreviewPage::slotTableDoubleClicked(QModelIndex index)
	{
		QString dataId;

		emit signalOpenAudioDataPage(dataId);
	}

	void DataPreviewPage::setPageNum(int page)
	{
	}

	void DataPreviewPage::setFixedPageRowCount(int pageItemCount)
	{
		m_tableModel->setFixedPageRowCount(pageItemCount);
	}

	void DataPreviewPage::init()
	{
		CustomWidgetContainer * cwidget = new CustomWidgetContainer();
		{
			m_operationToolsPage = new OperationToolsPage();
			connect(m_operationToolsPage, SIGNAL(buttPressed(OperationToolsPage::ButtType)), this, SLOT(respToolButtPressed(OperationToolsPage::ButtType)));
			
			QLabel * label = new QLabel();
			label->setText(QStringLiteral("侦查平台:"));
			label->setFont(QFont(QStringLiteral("微软雅黑"), 15));
// 			label->setMinimumSize(60, 35);
// 			label->setMaximumSize(60, 35);

			QComboBox * comboBox = new QComboBox();
// 			comboBox->setMinimumSize(60, 35);
// 			comboBox->setMaximumSize(60, 35);

			QStringList list;
			list << QStringLiteral("HXJ01") << QStringLiteral("HXJ02") << QStringLiteral("HXJ03");
			comboBox->addItems(list);

			QWidget * widget = new QWidget();
			QHBoxLayout * hLayout = new QHBoxLayout();
			hLayout->addWidget(m_operationToolsPage);
			hLayout->addStretch();
			hLayout->addWidget(label);
			hLayout->addWidget(comboBox);
			hLayout->setContentsMargins(0,0,0,0); 
			widget->setLayout(hLayout);
			cwidget->setContent(widget);
		}
	
		CustomWidgetContainer * ctableView = new CustomWidgetContainer();
		{
			m_tableView = new Base::RTableView();
			m_tableView->setFocusPolicy(Qt::NoFocus);
			m_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
			m_tableView->setSelectionMode(QAbstractItemView::SingleSelection);
			m_tableView->addColumnItem(Base::ColumnItem(T_Index, QStringLiteral("索引")));
			m_tableView->addColumnItem(Base::ColumnItem(T_TargetName, QStringLiteral("编号"), 140));
			m_tableView->addColumnItem(Base::ColumnItem(T_Edttime, QStringLiteral("录入时间"), 180));
			m_tableView->addColumnItem(Base::ColumnItem(T_Tonnage, QStringLiteral("试验名"), 180));
			m_tableView->addColumnItem(Base::ColumnItem(T_AxlesNumber, QStringLiteral("平台名")));
			m_tableView->addColumnItem(Base::ColumnItem(T_Datalength, QStringLiteral("数据时长")));
			m_tableView->addColumnItem(Base::ColumnItem(T_Type, QStringLiteral("类型")));
			
			connect(m_tableView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(slotTableDoubleClicked(QModelIndex)));

			m_tableModel = new DataOverviewMolel();
			m_tableView->setModel(m_tableModel);
			m_tableModel->prepareData();

	
			m_pageSwitch = new PageSwitchBar();
			m_pageSwitch->setDataSize(m_tableModel->datasSize());
			connect(m_pageSwitch, SIGNAL(perPageNumsChanged(int)), this, SLOT(setFixedPageRowCount(int)));
			connect(m_pageSwitch, SIGNAL(switchPage(int)), this, SLOT(setPageNum(int)));

			QWidget * twidget = new QWidget();
			QVBoxLayout * cvlayout = new QVBoxLayout();
			cvlayout->setContentsMargins(0, 0, 0, 0);
			cvlayout->addWidget(m_tableView);
			cvlayout->addWidget(m_pageSwitch);
			twidget->setLayout(cvlayout);

			ctableView->setContent(twidget);
		}


		QVBoxLayout * vlayout = new QVBoxLayout();
		vlayout->setContentsMargins(4, 4, 4, 4);
		vlayout->addWidget(cwidget);
		vlayout->addWidget(ctableView);
		setLayout(vlayout);
	}

	void DataPreviewPage::initConnect()
	{
	}

}//namespace Related 