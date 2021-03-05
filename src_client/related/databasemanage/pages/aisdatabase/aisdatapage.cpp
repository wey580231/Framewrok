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
		m_firstLoadData(true)
	{
		init();
		initConnect();

	}

	AisDataPage::~AisDataPage()
	{
	}

	PageType AisDataPage::getPageType() const
	{
		return Page_AisDataBase_AisData;
	}

	void AisDataPage::prepareBringToTop()
	{
		if (m_firstLoadData) {
			refreshCurrPage();
			m_firstLoadData = false;
		}
	}

	void AisDataPage::respToolButtPressed(OperationToolsPage::ButtType type)
	{
		switch (type)
		{
		case OperationToolsPage::Butt_Add: {
			createAISData();
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

	void AisDataPage::processQueryAllAISDataResponse(const Datastruct::LoadAllAISDatasResponse & response)
	{
		m_tableModel->prepareData(response.m_aisDataInfos);
		m_pageSwitch->setDataSize(response.m_aisDataCount);
	}

	void AisDataPage::processAISDataCreateResponse(const Datastruct::AISDataCreateResponse & response)
	{
		if (response.m_createResult) {
			refreshCurrPage();
		}
	}

	void AisDataPage::processAISDataDeleteResponse(const Datastruct::AISDataDeleteResponse & response)
	{
		if (response.m_deleteResult) {
			refreshCurrPage();
		}
	}

	void AisDataPage::processAISDataModifyResponse(const Datastruct::AISDataModifyResponse & response)
	{
		if (response.m_modifyResult) {
			refreshCurrPage();
		}
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
			m_tableView->addColumnItem(Base::ColumnItem(AIS_Index, QStringLiteral("索引")));
			m_tableView->addColumnItem(Base::ColumnItem(AIS_Mmsi, QStringLiteral("船舶MMSI"), 180));
			m_tableView->addColumnItem(Base::ColumnItem(AIS_Time, QStringLiteral("实时信号时间")));
			m_tableView->addColumnItem(Base::ColumnItem(AIS_Lon, QStringLiteral("经度")));
			m_tableView->addColumnItem(Base::ColumnItem(AIS_Lat, QStringLiteral("纬度")));
			m_tableView->addColumnItem(Base::ColumnItem(AIS_Course, QStringLiteral("航迹向")));
			m_tableView->addColumnItem(Base::ColumnItem(AIS_Truehead, QStringLiteral("航首向")));
			m_tableView->addColumnItem(Base::ColumnItem(AIS_Name, QStringLiteral("船名")));
			m_tableView->addColumnItem(Base::ColumnItem(AIS_ShipType, QStringLiteral("船舶以及货物类型"), 200));
			m_tableView->addColumnItem(Base::ColumnItem(AIS_NavStatus, QStringLiteral("航行状态")));
			m_tableView->addColumnItem(Base::ColumnItem(AIS_Speed, QStringLiteral("航行速度")));
			m_tableView->addColumnItem(Base::ColumnItem(AIS_Length, QStringLiteral("船长(m)")));
			m_tableView->addColumnItem(Base::ColumnItem(AIS_Width, QStringLiteral("船宽(m)")));
			m_tableView->addColumnItem(Base::ColumnItem(AIS_BuildDate, QStringLiteral("建造时间")));
			m_tableView->addColumnItem(Base::ColumnItem(AIS_Port, QStringLiteral("船籍港")));

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
		connect(SignalDispatch::instance(), SIGNAL(respQueryAllAISDataResponse(const Datastruct::LoadAllAISDatasResponse &)),
			this, SLOT(processQueryAllAISDataResponse(const Datastruct::LoadAllAISDatasResponse &)));
		connect(SignalDispatch::instance(), SIGNAL(respAISDataCreateResponse(const Datastruct::AISDataCreateResponse &)),
			this, SLOT(processAISDataCreateResponse(const Datastruct::AISDataCreateResponse &)));
		connect(SignalDispatch::instance(), SIGNAL(respAISDataDeleteResponse(const Datastruct::AISDataDeleteResponse &)),
			this, SLOT(processAISDataDeleteResponse(const Datastruct::AISDataDeleteResponse &)));
		connect(SignalDispatch::instance(), SIGNAL(respAISDataModifyResponse(const Datastruct::AISDataModifyResponse &)),
			this, SLOT(processAISDataModifyResponse(const Datastruct::AISDataModifyResponse &)));
	}

	void AisDataPage::refreshCurrPage()
	{
		Datastruct::LoadAllAISDataRequest request;
		request.m_offsetIndex = m_pageSwitch->dataOffset();
		request.m_limitIndex = m_pageSwitch->perPageCount();
		DataNetConnector::instance()->write(request);
	}

	void AisDataPage::createAISData()
	{
		Datastruct::AISDataCreateRequest request;
		request.m_id = Base::RUtil::UUID();					
		request.m_targetId = QStringLiteral("AIS0");				
		request.m_mmsi = 0;						
		request.m_time = 1;						
		request.m_lon = 2;					
		request.m_lat = 3;					
		request.m_course = 4;				
		request.m_truehead = 5;				
		request.m_name = QStringLiteral("AIS1");					
		request.m_shipType = 6;					
		request.m_shipImo = 7;					
		request.m_navStatus = 8;				
		request.m_speed = 9;					
		request.m_eta = QStringLiteral("2020-12-13 02:51:46");					
		request.m_dest = QStringLiteral("AIS3");					
		request.m_length = 10;				
		request.m_width = 11;					
		request.m_callsign = QStringLiteral("AIS4");				
		request.m_flag = QStringLiteral("AIS5");					
		request.m_buildDate = QStringLiteral("2020-12-13 02:51:46");
		request.m_port = QStringLiteral("AIS7");					

		DataNetConnector::instance()->write(request);
	}

} //namespace Related