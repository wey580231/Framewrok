#include "dutyrecordpage.h"

#include <QDebug>

#include "../../utils/util.h"
#include "../../customwidget/customwidgetcontainer.h"
#include "../../net/netconnector.h"
#include "../../net/signaldispatch.h"
#include "../../global.h"

namespace Related {

	DutyRecordPage::DutyRecordPage(QWidget *parent)
		: AbstractPage(parent),
		m_operationToolsPage(nullptr),
		m_tableView(nullptr),
		m_tableModel(nullptr),
		m_pageSwitch(nullptr),
		m_firstLoadData(true),
		m_seleteTableRow(99999999)
	{
		init();
		initConnect();
	}

	DutyRecordPage::~DutyRecordPage()
	{
	}

	PageType DutyRecordPage::getPageType() const
	{
		return Page_TaskRecord_DutyRecord;
	}

	void DutyRecordPage::prepareBringToTop()
	{
		if(m_firstLoadData){
			refreshCurrPage();
			m_firstLoadData = false;
		}
	}

	void  DutyRecordPage::setTaskId(QString taskId)
	{
		if (m_taskId.isEmpty()) {
			m_taskId = taskId;
		}
		else
		{
			if (m_taskId != taskId) {
				m_taskId = taskId;
				m_firstLoadData = true;
			}
		}
	}

	void DutyRecordPage::respToolButtPressed(OperationToolsPage::ButtType type)
	{
		switch (type)
		{
		case OperationToolsPage::Butt_Add: {
			insertDutyRecord();
		}
			break;
		case OperationToolsPage::Butt_Delete: {
				if (m_seleteTableRow < m_allDutyRecords.m_dutyRecordInfos.size()) {
					Datastruct::DutyRecordEntityData data = m_allDutyRecords.m_dutyRecordInfos.at(m_seleteTableRow);
					deleteDutyRecord(data.id);
					m_seleteTableRow = 99999999;
				}
		}
			break;
		case OperationToolsPage::Butt_Edit: {
			if (m_seleteTableRow < m_allDutyRecords.m_dutyRecordInfos.size()) {
				Datastruct::DutyRecordEntityData data = m_allDutyRecords.m_dutyRecordInfos.at(m_seleteTableRow);

				data.wind = 55;
				data.windSpeed = 55;
				data.waveHigh = 55;
				data.oceanCurrents = 55;

				modifyDutyRecord(data);
				m_seleteTableRow = 99999999;
			}
		}
			break;
		case OperationToolsPage::Butt_Refresh: {
			refreshCurrPage();
		}
			break;
		default:
			break;
		}
	}

	void DutyRecordPage::processDutyRecordCreateResponse(const Datastruct::DutyRecordCreateResponse & response)
	{
		if (response.m_createResult == true) {
			refreshCurrPage();
		}
	}

	void DutyRecordPage::processQueryAllDutyRecordResponse(const Datastruct::LoadAllDutyRecordResponse & response)
	{
		m_allDutyRecords = response;
		m_tableModel->prepareData(response.m_dutyRecordInfos);
		m_pageSwitch->setDataSize(response.m_dutyRecordCount);
	}

	void DutyRecordPage::processDutyRecordDeleteResponse(const Datastruct::DutyRecordDeleteResponse & response)
	{
		if (response.m_deleteResult) {
			refreshCurrPage();
		}
	}

	void DutyRecordPage::processDutyRecordModifyResponse(const Datastruct::DutyRecordModifyResponse & response)
	{
		if (response.m_modifyResult) {
			refreshCurrPage();
		}
	}

	void DutyRecordPage::slotClickedTable(QModelIndex index)
	{
		m_seleteTableRow = index.row();
	}

	void DutyRecordPage::init()
	{
		CustomWidgetContainer * cwidget = new CustomWidgetContainer();
		{
			m_operationToolsPage = new OperationToolsPage();
			connect(m_operationToolsPage, SIGNAL(buttPressed(OperationToolsPage::ButtType)), this, SLOT(respToolButtPressed(OperationToolsPage::ButtType)));
			cwidget->setContent(m_operationToolsPage);
		}

		CustomWidgetContainer * ctableView = new CustomWidgetContainer();
		{
			m_tableModel = new DutyRecordModel();

			m_tableView = new Base::RTableView();
			m_tableView->setFocusPolicy(Qt::NoFocus);
			m_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
			m_tableView->setSelectionMode(QAbstractItemView::SingleSelection);

			m_tableView->setModel(m_tableModel);
			m_tableView->addColumnItem(Base::ColumnItem(DR_Index, QStringLiteral("索引")));
			m_tableView->addColumnItem(Base::ColumnItem(DR_CreateTime, QStringLiteral("录入时间"), 180));
			m_tableView->addColumnItem(Base::ColumnItem(DR_Description, QStringLiteral("描述")));
			m_tableView->addColumnItem(Base::ColumnItem(DR_SeaCondition, QStringLiteral("海况")));
			m_tableView->addColumnItem(Base::ColumnItem(DR_Wind, QStringLiteral("风向")));
			m_tableView->addColumnItem(Base::ColumnItem(DR_WindSpeed, QStringLiteral("风速")));
			m_tableView->addColumnItem(Base::ColumnItem(DR_WaveHigh, QStringLiteral("浪高")));
			m_tableView->addColumnItem(Base::ColumnItem(DR_OceanCurrents, QStringLiteral("洋流")));
			
			connect(m_tableView, SIGNAL(clicked(QModelIndex)), this, SLOT(slotClickedTable(QModelIndex)));

			m_pageSwitch = new PageSwitchBar();
			m_pageSwitch->setDataSize(m_tableModel->datasSize());

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

	void DutyRecordPage::initConnect()
	{
		// 信号与槽
		connect(SignalDispatch::instance(), SIGNAL(respDutyRecordCreateResponse(const Datastruct::DutyRecordCreateResponse &)),
			this, SLOT(processDutyRecordCreateResponse(const Datastruct::DutyRecordCreateResponse &)));

		connect(SignalDispatch::instance(), SIGNAL(respQueryAllDutyRecordResponse(const Datastruct::LoadAllDutyRecordResponse &)),
			this, SLOT(processQueryAllDutyRecordResponse(const Datastruct::LoadAllDutyRecordResponse &)));
	
		connect(SignalDispatch::instance(), SIGNAL(respDutyRecordDeleteResponse(const Datastruct::DutyRecordDeleteResponse &)),
			this, SLOT(processDutyRecordDeleteResponse(const Datastruct::DutyRecordDeleteResponse &)));

		connect(SignalDispatch::instance(), SIGNAL(respDutyRecordModifyResponse(const Datastruct::DutyRecordModifyResponse &)),
			this, SLOT(processDutyRecordModifyResponse(const Datastruct::DutyRecordModifyResponse &)));
	}

	/*!
	 * @brief   插入一条记录
	 * @details 
	 */
	void DutyRecordPage::insertDutyRecord()
	{
		Datastruct::DutyRecordCreateRequest request;
		request.m_id = Base::RUtil::UUID();
		request.m_taskId = m_taskId;
		QDateTime current_date_time = QDateTime::currentDateTime();
		request.m_createTime = current_date_time.toString("yyyy.MM.dd hh:mm:ss.zzz");			
		request.m_description = QStringLiteral("1");
		request.m_seaCondition; QStringLiteral("1");
		request.m_wind = 0;						
		request.m_windSpeed = 0;					
		request.m_waveHigh = 0;					
		request.m_oceanCurrents = 0;				

		NetConnector::instance()->write(request);
	}

	void DutyRecordPage::deleteDutyRecord(QString id)
	{
		Datastruct::DutyRecordDeleteRequest request;
		request.m_id = id;
		NetConnector::instance()->write(request);
	}

	void DutyRecordPage::modifyDutyRecord(Datastruct::DutyRecordEntityData info)
	{
		Datastruct::DutyRecordModifyRequest request;
		request.m_id = info.id;
		request.m_taskId = info.taskId;
		request.m_createTime = info.createTime;
		request.m_description = info.description;
		request.m_seaCondition = info.seaCondition;
		request.m_wind = info.wind;
		request.m_windSpeed = info.windSpeed;
		request.m_waveHigh = info.waveHigh;
		request.m_oceanCurrents = info.oceanCurrents;

		NetConnector::instance()->write(request);
	}

	void DutyRecordPage::refreshCurrPage()
	{
		Datastruct::LoadAllDutyRecordRequest request;
		request.m_taskId = m_taskId;
		request.m_offsetIndex = m_pageSwitch->dataOffset();
		request.m_limitIndex = m_pageSwitch->perPageCount();
		NetConnector::instance()->write(request);
	}

}//namespace Related