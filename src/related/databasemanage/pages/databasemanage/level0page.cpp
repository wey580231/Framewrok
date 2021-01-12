#include "level0page.h"

#include <QHBoxLayout>
#include <QDateTimeEdit>
#include <QLabel>
#include <QListView>
#include <QDebug>

namespace Related {

	Level0Page::Level0Page(QWidget *parent)
		: QWidget(parent)
	{
		init();
	}

	Level0Page::~Level0Page()
	{

	}

	void Level0Page::init()
	{
		m_tableView = new Base::RTableView();
		m_tableView->setFocusPolicy(Qt::NoFocus);
		m_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
		m_tableView->setSelectionMode(QAbstractItemView::SingleSelection);

		m_tableView->addColumnItem(Base::ColumnItem(C_Id, QStringLiteral("索引")));
		m_tableView->addColumnItem(Base::ColumnItem(C_Num, QStringLiteral("编号")));
		m_tableView->addColumnItem(Base::ColumnItem(C_Timestamp, QStringLiteral("录入时间")));
		m_tableView->addColumnItem(Base::ColumnItem(C_ExperienceName, QStringLiteral("实验名")));
		m_tableView->addColumnItem(Base::ColumnItem(C_PlatformName, QStringLiteral("平台名")));
		m_tableView->addColumnItem(Base::ColumnItem(C_TimeLength, QStringLiteral("数据时长(s)")));
		m_tableView->addColumnItem(Base::ColumnItem(C_DataType, QStringLiteral("数据类型")));

		m_tableModel = new LevelModel0();
		m_tableModel->prepareData();

		m_tableView->setModel(m_tableModel);

		auto createButt = [](QString icon,QString text) {
			Base::RIconButton * butt = new Base::RIconButton();
			butt->setIconSize(Base::RIconButton::ICON_16);
			butt->setIcon(QIcon(icon));
			butt->setText(text);
			butt->setFont(QFont(QStringLiteral("微软雅黑"),10));
			butt->setMinimumSize(60, 30);

			return butt;
		};

		m_addButton = createButt(QStringLiteral(":/QYBlue/resource/qyblue/新增.png"),QStringLiteral("新增"));
		m_delButton = createButt(QStringLiteral(":/QYBlue/resource/qyblue/删除.png"),QStringLiteral("删除"));
		m_editButton = createButt(QStringLiteral(":/QYBlue/resource/qyblue/编辑.png"),QStringLiteral("编辑"));
		m_refreshButton = createButt(QStringLiteral(":/QYBlue/resource/qyblue/刷新.png"),QStringLiteral("刷新"));

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

		m_searchButton = createButt(QStringLiteral(":/QYBlue/resource/qyblue/搜索.png"), QStringLiteral("搜索"));
		m_advanceSearchButton = createButt(QStringLiteral(":/QYBlue/resource/qyblue/高级搜索.png"), QStringLiteral("高级搜索"));
		m_moreButton = createButt(QStringLiteral(":/QYBlue/resource/qyblue/更多.png"), QStringLiteral("更多"));

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
		QWidget * pageSwitch = new QWidget();
		pageSwitch->setObjectName("mainWidget");
		pageSwitch->setFixedHeight(35);
		QHBoxLayout * pageLayout = new QHBoxLayout();
		pageLayout->setContentsMargins(4, 4, 4, 4);
		
		m_firstButton = createButt(QStringLiteral(":/QYBlue/resource/qyblue/到首页.png"),QStringLiteral("首页"));
		m_prePageButton = createButt(QStringLiteral(":/QYBlue/resource/qyblue/小于号.png"),QStringLiteral("上页"));
		m_nextPageButton = createButt(QStringLiteral(":/QYBlue/resource/qyblue/大于号.png"),QStringLiteral("下页"));
		m_lastButton = createButt(QStringLiteral(":/QYBlue/resource/qyblue/到尾页.png"),QStringLiteral("尾页"));

		pageLayout->addStretch(1);

		m_pageItemCountBox = new QComboBox();
		m_pageItemCountBox->setFocusPolicy(Qt::NoFocus);
		m_pageItemCountBox->setView(new QListView());
		m_pageItemCountBox->addItem(QStringLiteral("30条/页"));
		m_pageItemCountBox->addItem(QStringLiteral("50条/页"));
		m_pageItemCountBox->addItem(QStringLiteral("100条/页"));

		pageLayout->addWidget(m_pageItemCountBox);
		pageLayout->addSpacing(15);
		pageLayout->addWidget(m_firstButton);
		pageLayout->addWidget(m_prePageButton);
		pageLayout->addWidget(m_nextPageButton);
		pageLayout->addWidget(m_lastButton);
		pageLayout->addStretch(1);

		pageSwitch->setLayout(pageLayout);

		QVBoxLayout * vlayout = new QVBoxLayout();
		vlayout->setContentsMargins(4, 4, 4, 4);
		vlayout->addWidget(toolWidget);
		vlayout->addWidget(m_tableView);
		vlayout->addWidget(pageSwitch);

		setLayout(vlayout);
	}

} //namespace Related 