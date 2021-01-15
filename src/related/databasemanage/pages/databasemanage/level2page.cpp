#include "level2page.h"

#include <QHBoxLayout>
#include <QDateTimeEdit>
#include <QLabel>
#include <QListView>
#include <QDebug>

#include "../util.h"
#include "../customwidget/pageswitchbar.h"

namespace Related {

	Level2Page::Level2Page(QWidget *parent)
		: QWidget(parent)
	{
		init();
	}

	Level2Page::~Level2Page()
	{

	}

	void Level2Page::setPageNum(int newPage)
	{
		m_tableModel->setPageNum(newPage);
		m_tableModel->refresh();
	}

	void Level2Page::init()
	{
		m_tableView = new Base::RTableView();
		m_tableView->setFocusPolicy(Qt::NoFocus);
		m_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
		m_tableView->setSelectionMode(QAbstractItemView::SingleSelection);

		m_tableView->addColumnItem(Base::ColumnItem(L2_Id, QStringLiteral("索引")));
		m_tableView->addColumnItem(Base::ColumnItem(L2_Num, QStringLiteral("批号")));
		m_tableView->addColumnItem(Base::ColumnItem(L2_Timestamp, QStringLiteral("时间")));
		m_tableView->addColumnItem(Base::ColumnItem(L2_Type, QStringLiteral("类型")));
		m_tableView->addColumnItem(Base::ColumnItem(L2_JyNum, QStringLiteral("桨叶数")));
		m_tableView->addColumnItem(Base::ColumnItem(L2_ZxDegree, QStringLiteral("置信度")));
		m_tableView->addColumnItem(Base::ColumnItem(L2_Direction, QStringLiteral("方位")));
		m_tableView->addColumnItem(Base::ColumnItem(L2_Nation, QStringLiteral("国籍")));

		m_tableModel = new LevelModel0();
		m_tableModel->prepareData();

		m_tableView->setModel(m_tableModel);

		m_addButton = Util::createButt(QStringLiteral(":/QYBlue/resource/qyblue/新增.png"),QStringLiteral("新增"));
		m_delButton = Util::createButt(QStringLiteral(":/QYBlue/resource/qyblue/删除.png"),QStringLiteral("删除"));
		m_editButton = Util::createButt(QStringLiteral(":/QYBlue/resource/qyblue/编辑.png"),QStringLiteral("编辑"));
		m_refreshButton = Util::createButt(QStringLiteral(":/QYBlue/resource/qyblue/刷新.png"),QStringLiteral("刷新"));

		//时间筛选
		QWidget * timeRangeSelect = new QWidget();
		QHBoxLayout * timeLayout = new QHBoxLayout();

		QLabel * inputTime = new QLabel();
		inputTime->setText(QStringLiteral("录入时间:"));

		QLabel * toTime = new QLabel();
		toTime->setText(QStringLiteral(" 至 "));

		QDateTimeEdit * startTimeEdit = new QDateTimeEdit();
		startTimeEdit->setMinimumHeight(25);

		QDateTimeEdit * endTimeEdit = new QDateTimeEdit();
		endTimeEdit->setMinimumHeight(25);

		timeLayout->addWidget(inputTime);
		timeLayout->addWidget(startTimeEdit);
		timeLayout->addWidget(toTime);
		timeLayout->addWidget(endTimeEdit);

		timeRangeSelect->setLayout(timeLayout);

		m_searchButton = Util::createButt(QStringLiteral(":/QYBlue/resource/qyblue/搜索.png"), QStringLiteral("搜索"));
		m_advanceSearchButton = Util::createButt(QStringLiteral(":/QYBlue/resource/qyblue/高级搜索.png"), QStringLiteral("高级搜索"));
		m_moreButton = Util::createButt(QStringLiteral(":/QYBlue/resource/qyblue/更多.png"), QStringLiteral("更多"));

		QWidget * toolWidget = new QWidget();
		QHBoxLayout * toolLayout = new QHBoxLayout();
		toolLayout->setContentsMargins(0, 0, 0, 0);

		toolLayout->addWidget(m_addButton);
		toolLayout->addWidget(m_delButton);
		toolLayout->addWidget(m_editButton);
		toolLayout->addWidget(m_refreshButton);
		toolLayout->addStretch(1);

		toolLayout->addWidget(timeRangeSelect);
		toolLayout->addWidget(m_searchButton);
		toolLayout->addWidget(m_advanceSearchButton);
		toolLayout->addWidget(m_moreButton);

		toolWidget->setLayout(toolLayout);

		//页面切换
		PageSwitchBar * pageSwitch = new PageSwitchBar();
		pageSwitch->setDataSize(m_tableModel->datasSize());
		connect(pageSwitch, SIGNAL(perPageNumsChanged(int)), m_tableModel, SLOT(setFixedPageRowCount(int)));
		connect(pageSwitch, SIGNAL(switchPage(int)), this, SLOT(setPageNum(int)));

		QVBoxLayout * vlayout = new QVBoxLayout();
		vlayout->setContentsMargins(4, 4, 4, 4);
		vlayout->addWidget(toolWidget);
		vlayout->addWidget(m_tableView);
		vlayout->addWidget(pageSwitch);

		setLayout(vlayout);
	}

} //namespace Related 