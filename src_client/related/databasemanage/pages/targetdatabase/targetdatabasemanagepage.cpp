#include "targetdatabasemanagepage.h"

#include <QDebug>
#include <QComboBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QListView>

#include "../../utils/util.h"
#include "../../customwidget/customwidgetcontainer.h"

namespace Related {

	TargetDatabaseManagePage::TargetDatabaseManagePage(QWidget *parent)
		: AbstractPage(parent)
	{
		init();
		initConnect();
	}

	TargetDatabaseManagePage::~TargetDatabaseManagePage()
	{
	}

	PageType TargetDatabaseManagePage::getPageType() const
	{
		return Page_TargetDatabase_TargetOverview;
	}

	void TargetDatabaseManagePage::respToolButtPressed(OperationToolsPage::ButtType type)
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

	void TargetDatabaseManagePage::setPageNum(int page)
	{
	}

	void TargetDatabaseManagePage::setFixedPageRowCount(int pageItemCount)
	{
		m_tableModel->setFixedPageRowCount(pageItemCount);
	}

	void TargetDatabaseManagePage::init()
	{
		CustomWidgetContainer * cwidget = new CustomWidgetContainer();
		{
			m_operationToolsPage = new OperationToolsPage();
			connect(m_operationToolsPage, SIGNAL(buttPressed(OperationToolsPage::ButtType)),
				this, SLOT(respToolButtPressed(OperationToolsPage::ButtType)));

			//临时测试代码
			QWidget * conditionFilterWidget = new QWidget();
			QHBoxLayout * filterLayout = new QHBoxLayout();
			filterLayout->setContentsMargins(0, 0, 4, 0);
			conditionFilterWidget->setLayout(filterLayout);

			auto createLabel = [](QString text) {
				QLabel * label = new QLabel();
				label->setText(text);
				return label;
			};

			filterLayout->addWidget(createLabel(QStringLiteral("目标种类")));

			QComboBox * ct = new QComboBox();
			ct->setView(new QListView());
			QStringList ctList;
			ctList << QStringLiteral("全部种类") << QStringLiteral("军用") << QStringLiteral("民用") << QStringLiteral("其它");
			ct->addItems(ctList);

			filterLayout->addWidget(ct);
			filterLayout->addWidget(createLabel(QStringLiteral("类型")));

			QComboBox * category = new QComboBox();
			category->setView(new QListView());
			QStringList cateList;
			cateList << QStringLiteral("全部类型") << QStringLiteral("水面舰") << QStringLiteral("商船") << QStringLiteral("渔船");
			category->addItems(cateList);
			
			filterLayout->addWidget(category);
			filterLayout->addWidget(createLabel(QStringLiteral("型号")));

			QComboBox * type = new QComboBox();
			type->setView(new QListView());
			QStringList tList;
			tList << QStringLiteral("全部型号") << QStringLiteral("XX型") << QStringLiteral("XXA型");
			type->addItems(tList);

			filterLayout->addWidget(type);

			QWidget * tmpWidget = new QWidget();
			QHBoxLayout * tmpLayout = new QHBoxLayout();
			tmpLayout->setContentsMargins(0, 0, 0, 0);
			tmpLayout->addWidget(m_operationToolsPage);
			tmpLayout->addStretch(1);
			tmpLayout->addWidget(conditionFilterWidget);
			tmpWidget->setLayout(tmpLayout);
			cwidget->setContent(tmpWidget);
		}

		CustomWidgetContainer * ctableView = new CustomWidgetContainer();
		{
			m_tableView = new Base::RTableView();
			m_tableView->setFocusPolicy(Qt::NoFocus);
			m_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
			m_tableView->setSelectionMode(QAbstractItemView::SingleSelection);

			m_tableModel = new TargetDatabaseManageModel();
			m_tableModel->prepareData();

			m_tableView->setModel(m_tableModel);

			m_tableView->addColumnItem(Base::ColumnItem(T_Index, QStringLiteral("索引")));
			m_tableView->addColumnItem(Base::ColumnItem(T_TargetName, QStringLiteral("目标名称"), 140));
			m_tableView->addColumnItem(Base::ColumnItem(T_Edttime, QStringLiteral("录人时间"), 180));
			m_tableView->addColumnItem(Base::ColumnItem(T_Tonnage, QStringLiteral("吨位(t)"), 180));
			m_tableView->addColumnItem(Base::ColumnItem(T_Lon, QStringLiteral("经度"), 180));
			m_tableView->addColumnItem(Base::ColumnItem(T_Lat, QStringLiteral("纬度"), 180));
			m_tableView->addColumnItem(Base::ColumnItem(T_Speed, QStringLiteral("航行速度"), 180));
			m_tableView->addColumnItem(Base::ColumnItem(T_AxlesNumber, QStringLiteral("轴数")));
			m_tableView->addColumnItem(Base::ColumnItem(T_Type, QStringLiteral("类型")));

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

	void TargetDatabaseManagePage::initConnect()
	{

	}

}//namespace Related 
