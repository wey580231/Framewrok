#include "datasamplepage.h"

#include <QHBoxLayout>
#include <QDateTimeEdit>
#include <QLabel>
#include <QListView>
#include <QDebug>

#include "../customwidget/pageswitchbar.h"

namespace Related {

	datasamplepage::datasamplepage(QWidget *parent)
		: QWidget(parent)
	{
		init();
		crerateAction();
	}

	datasamplepage::~datasamplepage()
	{
	}

	void datasamplepage::contextMenuEvent(QContextMenuEvent * event)
	{
		m_pop_menu->clear();
		m_pop_menu->addAction(m_hcAction);
		m_pop_menu->addAction(m_csbAction);
		m_pop_menu->addAction(m_swtzAction);
		m_pop_menu->addAction(m_mbxxAction);
		m_pop_menu->addAction(m_sbxxAction);
		m_pop_menu->addAction(m_scAction);
		m_pop_menu->exec(QCursor::pos());
		event->accept();
	}

	void datasamplepage::slotLeadingInButton()
	{
		emit signalDataGather();
	}

	void datasamplepage::slotInspectAction()
	{
		emit signalInspectData();
	}

	void datasamplepage::init()
	{
		auto createButt = [](QString icon, QString text) {
			Base::RIconButton * butt = new Base::RIconButton();
			butt->setIconSize(Base::RIconButton::ICON_16);
			butt->setIcon(QIcon(icon));
			butt->setText(text);
			butt->setFont(QFont(QStringLiteral("微软雅黑"), 10));
			butt->setMinimumSize(60, 30);
			return butt;
		};

		m_leadingInButton = createButt(QStringLiteral(":/QYBlue/resource/qyblue/新增.png"), QStringLiteral("导入"));
		m_gatherButton = createButt(QStringLiteral(":/QYBlue/resource/qyblue/新增.png"), QStringLiteral("采集"));

		connect(m_gatherButton, SIGNAL(clicked()), this, SLOT(slotLeadingInButton()));

		m_identificationQueueButton = createButt(QStringLiteral(":/QYBlue/resource/qyblue/新增.png"), QStringLiteral("识别队列"));
		m_retrievalButton = createButt(QStringLiteral(":/QYBlue/resource/qyblue/刷新.png"), QStringLiteral("检索"));
		m_congnizanceButton = createButt(QStringLiteral(":/QYBlue/resource/qyblue/新增.png"), QStringLiteral("同一性认定"));
		m_batchDeleButton = createButt(QStringLiteral(":/QYBlue/resource/qyblue/删除.png"), QStringLiteral("批量删除"));
		QWidget *totalWidget = new QWidget();
		QHBoxLayout *t_totalLayout = new QHBoxLayout();
		t_totalLayout->addWidget(m_leadingInButton);
		t_totalLayout->addWidget(m_gatherButton);
		t_totalLayout->addWidget(m_identificationQueueButton);
		t_totalLayout->addWidget(m_retrievalButton);
		t_totalLayout->addWidget(m_congnizanceButton);
		t_totalLayout->addWidget(m_batchDeleButton);
		t_totalLayout->addStretch();
		t_totalLayout->setContentsMargins(0,0,0,0);
		t_totalLayout->setSpacing(1);
		totalWidget->setLayout(t_totalLayout);
		
		//
		m_tableView = new Base::RTableView();
		m_tableView->setFocusPolicy(Qt::NoFocus);

		m_tableModel = new datasamplemodel();
		m_tableModel->prepareData();
		m_tableView->setModel(m_tableModel);

		m_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
		m_tableView->setSelectionMode(QAbstractItemView::SingleSelection);
		m_tableView->addColumnItem(Base::ColumnItem(D_Id, QStringLiteral("序号"),70));
		m_tableView->addColumnItem(Base::ColumnItem(D_Time, QStringLiteral("时间"),150));
		m_tableView->addColumnItem(Base::ColumnItem(D_BatchNumber, QStringLiteral("批号")));
		m_tableView->addColumnItem(Base::ColumnItem(D_LocalDistinguish, QStringLiteral("本地识别")));
		m_tableView->addColumnItem(Base::ColumnItem(D_Result, QStringLiteral("同一认定结果")));

		m_tableView->setContentsMargins(0, 0, 0, 0);

		PageSwitchBar * pageSwitch = new PageSwitchBar();
		connect(pageSwitch, SIGNAL(perPageNumsChanged(int)), m_tableModel, SLOT(setFixedPageRowCount(int)));

		QVBoxLayout * mainVLayout = new QVBoxLayout();
		mainVLayout->addWidget(totalWidget);
		mainVLayout->addWidget(m_tableView);
		mainVLayout->addWidget(pageSwitch);
		mainVLayout->setContentsMargins(0,0,0,0);
		mainVLayout->setSpacing(1);
		setLayout(mainVLayout);
	}

	void datasamplepage::crerateAction()
	{
		m_pop_menu = new QMenu();
		m_hcAction = new QAction(this);
		m_hcAction->setText(QStringLiteral("核查"));
		connect(m_hcAction, SIGNAL(triggered()), this,SLOT(slotInspectAction()));

		m_csbAction = new QAction(this);
		m_csbAction->setText(QStringLiteral("重识别"));
		m_swtzAction = new QAction(this);
		m_swtzAction->setText(QStringLiteral("声纹特征编辑"));
		m_mbxxAction = new QAction(this);
		m_mbxxAction->setText(QStringLiteral("目标信息修改"));
		m_sbxxAction = new QAction(this);
		m_sbxxAction->setText(QStringLiteral("识别信息上报"));
		m_scAction = new QAction(this);
		m_scAction->setText(QStringLiteral("删除"));
	}

}//namespace Related 