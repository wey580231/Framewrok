#include "aisdatapage.h"

#include <QDebug>

#include "../../utils/util.h"
#include "../../customwidget/customwidgetcontainer.h"
#include "../../net/datanetconnector.h"
#include "../../net/signaldispatch.h"
#include "../../global.h"

namespace Related {

	AisDataPage::AisDataPage(QWidget *parent)
		: AbstractPage(parent),
		m_firstLoadData(false)
	{
		init();
		initConnect();
		createData();
	}

	AisDataPage::~AisDataPage()
	{
	}

	PageType AisDataPage::getPageType() const
	{
		return Page_AisDataBase_AisData;
	}

	void AisDataPage::respToolButtPressed(OperationToolsPage::ButtType type)
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

	void AisDataPage::setPageNum(int page)
	{
	}

	void AisDataPage::setFixedPageRowCount(int pageItemCount)
	{
		m_tableModel->setFixedPageRowCount(pageItemCount);
	}

	void AisDataPage::init()
	{
		CustomWidgetContainer * cwidget = new CustomWidgetContainer();
		{
			m_operationToolsPage = new OperationToolsPage();
			connect(m_operationToolsPage, SIGNAL(buttPressed(OperationToolsPage::ButtType)), this, SLOT(respToolButtPressed(OperationToolsPage::ButtType)));
			cwidget->setContent(m_operationToolsPage);
		}

		CustomWidgetContainer * ctableView = new CustomWidgetContainer();
		{
			m_tableModel = new AisDataModel();

			m_tableView = new Base::RTableView();
			m_tableView->setFocusPolicy(Qt::NoFocus);
			m_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
			m_tableView->setSelectionMode(QAbstractItemView::SingleSelection);

			m_tableView->setModel(m_tableModel);
			m_tableView->addColumnItem(Base::ColumnItem(AIS_INDEX, QStringLiteral("索引")));
			m_tableView->addColumnItem(Base::ColumnItem(AIS_MMSI, QStringLiteral("船舶MMSI"), 180));
			m_tableView->addColumnItem(Base::ColumnItem(AIS_TIME, QStringLiteral("实时信号时间")));
			m_tableView->addColumnItem(Base::ColumnItem(AIS_LON, QStringLiteral("经度")));
			m_tableView->addColumnItem(Base::ColumnItem(AIS_LAT, QStringLiteral("纬度")));
			m_tableView->addColumnItem(Base::ColumnItem(AIS_COURSE, QStringLiteral("航迹向")));
			m_tableView->addColumnItem(Base::ColumnItem(AIS_TRUEHEAD, QStringLiteral("航首向")));
			m_tableView->addColumnItem(Base::ColumnItem(AIS_NAME, QStringLiteral("船名")));
			m_tableView->addColumnItem(Base::ColumnItem(AIS_SHIPTYPE, QStringLiteral("船舶以及货物类型"), 200));
			m_tableView->addColumnItem(Base::ColumnItem(AIS_NAVSTATUS, QStringLiteral("航行状态")));
			m_tableView->addColumnItem(Base::ColumnItem(AIS_SPEED, QStringLiteral("航行速度")));
			m_tableView->addColumnItem(Base::ColumnItem(AIS_LENGTH, QStringLiteral("船长(m)")));
			m_tableView->addColumnItem(Base::ColumnItem(AIS_WIDTH, QStringLiteral("船宽(m)")));
			m_tableView->addColumnItem(Base::ColumnItem(AIS_BUILDDATE, QStringLiteral("建造时间")));
			m_tableView->addColumnItem(Base::ColumnItem(AIS_PORT, QStringLiteral("船籍港")));

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

	void AisDataPage::initConnect()
	{
	}

	void AisDataPage::createData()
	{
		QList<Datastruct::AisEntityData> listData;
		for (int i = 0; i < 15;  i++) {
			QDateTime current_date_time = QDateTime::currentDateTime();

			Datastruct::AisEntityData data;
			data.mmsi = 0;					/*!< 船舶MMSI */
			data.time = 1600;					/*!< 实时信号时间 */
			data.lon = 15;					/*!< 经度 */
			data.lat = 16;					/*!< 纬度 */
			data.course = 16;				/*!< 航迹向 */
			data.truehead= 15+1;				/*!< 航首向 */
			data.name = QString("%1_%2").arg(QStringLiteral("中国")).arg(i+1);				/*!< 船名 */
			data.navStatus = 1;				/*!< 航行状态 */
			data.speed =16;				/*!< 航行速度 */
			data.length = 160;				/*!< 船长 */
			data.width = 43;				/*!< 船宽 */
			data.buildDate = current_date_time.toString(TIME_FORMAT);			/*!< 建造时间 */
			data.port = QStringLiteral("中国");				/*!< 船籍港 */

			listData.append(data);
		}

		m_tableModel->prepareData(listData);
	}

} //namespace Related