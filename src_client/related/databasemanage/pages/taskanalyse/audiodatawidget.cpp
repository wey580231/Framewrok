#include "audiodatawidget.h"

#include <QDebug>
#include <QHBoxLayout>

#include "../../utils/util.h"
#include "../../customwidget/pageswitchbar.h"
#include "../../customwidget/customwidgetcontainer.h"

namespace Related {

	WavDataPage::WavDataPage(QWidget *parent)
		: AbstractPage(parent)
	{
		init();
	}

	WavDataPage::~WavDataPage()
	{
	}

	PageType WavDataPage::getPageType() const
	{
		return Page_TaskAnalyse_WavData;
	}

	void WavDataPage::prepareBringToTop()
	{
	}

	void WavDataPage::setTaskId(QString taskId)
	{
	}


	void WavDataPage::slotTableDoubleClicked(QModelIndex index)
	{
		//m_StackedWidget->setCurrentIndex(1);
	}

	void WavDataPage::respButtCliecked()
	{
		m_StackedWidget->setCurrentIndex(0);
	}

	void WavDataPage::init()
	{
		m_StackedWidget = new QStackedWidget();
		
		//音频数据表格
		QWidget * dataTableWidget = new QWidget();
		{
			m_operationToolsPage = new OperationToolsPage();

			m_tableView = new Base::RTableView();
			m_tableView->setFocusPolicy(Qt::NoFocus);
			m_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
			m_tableView->setSelectionMode(QAbstractItemView::SingleSelection);
			m_tableModel = new AudioDataModel();
			m_tableModel->prepareData();

			m_tableView->setModel(m_tableModel);
			m_tableView->addColumnItem(Base::ColumnItem(T_Index, QStringLiteral("索引")));
			m_tableView->addColumnItem(Base::ColumnItem(T_TargetName, QStringLiteral("文件名"), 140));
			m_tableView->addColumnItem(Base::ColumnItem(T_Edttime, QStringLiteral("时长"), 180));
			m_tableView->addColumnItem(Base::ColumnItem(T_Tonnage, QStringLiteral("平台名称"), 180));
			m_tableView->addColumnItem(Base::ColumnItem(T_AxlesNumber, QStringLiteral("数据长度")));
			
			connect(m_tableView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(slotTableDoubleClicked(QModelIndex)));

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
			vlayout->setContentsMargins(0, 0, 0, 0);
			vlayout->addWidget(cwidget);
			vlayout->addWidget(ctableView);
			dataTableWidget->setLayout(vlayout);
		}

		QWidget * t_spectrogramWidget = new QWidget();
		{
			QLabel * label1 = new QLabel();
			QLabel * label2 = new QLabel();
			label1->setText(QStringLiteral("侦查平台："));
			label2->setText(QStringLiteral("HXJ_01"));


			QWidget *titleWidget = new QWidget();
			Base::RIconButton * quitButton = new Base::RIconButton();
			quitButton->setText(QStringLiteral("退出"));
			quitButton->setFont(QFont(QStringLiteral("微软雅黑"), 10));
			quitButton->setMinimumSize(40, 30);
			connect(quitButton, SIGNAL(pressed()), this, SLOT(respButtCliecked()));

			QHBoxLayout * hLayout = new QHBoxLayout();
			hLayout->addWidget(label1);
			hLayout->addWidget(label2);
			hLayout->addStretch();
		//	hLayout->addWidget(quitButton);
			titleWidget->setLayout(hLayout);
			titleWidget->setMinimumHeight(48);
			titleWidget->setMaximumHeight(48);

			CustomWidgetContainer * View1 = new CustomWidgetContainer();
			View1->setMaximumHeight(300);


			CustomWidgetContainer * View2 = new CustomWidgetContainer();


			CustomWidgetContainer * View3 = new CustomWidgetContainer();

			QWidget *t_widget = new QWidget();
			QHBoxLayout * hLayout2 = new QHBoxLayout();
			hLayout2->addWidget(View2);
			hLayout2->addWidget(View3);
			hLayout2->setContentsMargins(0,0,0,0);
			t_widget->setLayout(hLayout2);

			QVBoxLayout *vLayout = new QVBoxLayout();
			vLayout->addWidget(titleWidget);
			vLayout->addWidget(View1);
			vLayout->addWidget(t_widget);
			vLayout->setContentsMargins(0, 0, 0, 0);
			t_spectrogramWidget->setLayout(vLayout);
		}

		//m_StackedWidget->addWidget(dataTableWidget);
		m_StackedWidget->addWidget(t_spectrogramWidget);

		QVBoxLayout * mainlayout = new QVBoxLayout();
		mainlayout->setContentsMargins(4, 4, 4, 4);
		mainlayout->addWidget(m_StackedWidget);
		setLayout(mainlayout);
	}

}//namespace Related 