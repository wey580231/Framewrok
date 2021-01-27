#include "trialsheetwidget.h"

#include <QDebug>

#include "../utils/util.h"
#include "../customwidget/customwidgetcontainer.h"
#include "../net/netconnector.h"
#include "../net/signaldispatch.h"
#include "../global.h"

namespace Related {

	TrialSheetWidget::TrialSheetWidget(QWidget *parent)
		: AbstractPage(parent),
		m_firstLoadData(true),
		m_seleteTableRow(99999999)
	{
		init();
		initConnent();
	}

	TrialSheetWidget::~TrialSheetWidget()
	{
	}

	PageType TrialSheetWidget::getPageType() const
	{
		return Page_TaskRecord_ExperimentRecord;
	}

	void TrialSheetWidget::prepareBringToTop()
	{
		if (m_firstLoadData) {
			refreshCurrPage();
			m_firstLoadData = false;
		}
	}

	void  TrialSheetWidget::setTaskId(QString taskId)
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

	void TrialSheetWidget::respToolButtPressed(OperationToolsPage::ButtType type) 
	{
		switch (type)
		{
		case OperationToolsPage::Butt_Add: {
			insertExperimentRecord();
		}
			break;

		case OperationToolsPage::Butt_Delete: {
			if (m_seleteTableRow < m_allExperimentRecords.m_listInfos.size()) {
				Datastruct::ExperimentRecordEntityData data = m_allExperimentRecords.m_listInfos.at(m_seleteTableRow);
				deleteExperimentRecord(data.id);
				m_seleteTableRow = 99999999;
			}
		}
			break;

		case OperationToolsPage::Butt_Edit: {

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

	void TrialSheetWidget::processExperimentRecordCreateResponse(const Datastruct::ExperimentRecordCreateResponse & response)
	{
		if (response.m_createResult == true) {
			refreshCurrPage();
		}
	}

	void TrialSheetWidget::processQueryAllExperimentRecordResponse(const Datastruct::LoadAllExperimentRecordsResponse & response)
	{
		m_allExperimentRecords = response;
		m_tableModel->prepareData(response.m_listInfos);
		m_pageSwitch->setDataSize(response.m_count);
	}

	void TrialSheetWidget::processExperimentRecordDeleteResponse(const Datastruct::ExperimentRecordDeleteResponse & response)
	{
		if (response.m_deleteResult) {
			refreshCurrPage();
		}
	}

	void TrialSheetWidget::slotClickedTable(QModelIndex index)
	{
		m_seleteTableRow = index.row();
	}

	void TrialSheetWidget::init()
	{
		CustomWidgetContainer * cwidget = new CustomWidgetContainer();
		{
			m_operationToolsPage = new OperationToolsPage();
			connect(m_operationToolsPage, SIGNAL(buttPressed(OperationToolsPage::ButtType)), this, SLOT(respToolButtPressed(OperationToolsPage::ButtType)));
			cwidget->setContent(m_operationToolsPage);
		}

		CustomWidgetContainer * ctableView = new CustomWidgetContainer();
		{
			m_tableView = new Base::RTableView();
			m_tableView->setFocusPolicy(Qt::NoFocus);
			m_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
			m_tableView->setSelectionMode(QAbstractItemView::SingleSelection);
			connect(m_tableView, SIGNAL(clicked(QModelIndex)), this, SLOT(slotClickedTable(QModelIndex)));

			m_tableModel = new TrialSheetModel();

			m_tableView->setModel(m_tableModel);

			m_tableView->addColumnItem(Base::ColumnItem(T_Index, QStringLiteral("索引")));
			m_tableView->addColumnItem(Base::ColumnItem(TS_PlatformId, QStringLiteral("平台名"), 140));
			m_tableView->addColumnItem(Base::ColumnItem(TS_Lon, QStringLiteral("经度")));
			m_tableView->addColumnItem(Base::ColumnItem(TS_Lat, QStringLiteral("纬度")));
			m_tableView->addColumnItem(Base::ColumnItem(TS_SeaCondition, QStringLiteral("海况"), 180));
			m_tableView->addColumnItem(Base::ColumnItem(TS_FloatingTime, QStringLiteral("浮动时间"), 180));

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

	void TrialSheetWidget::initConnent()
	{
		connect(SignalDispatch::instance(), SIGNAL(respExperimentRecordCreateResponse(const Datastruct::ExperimentRecordCreateResponse &)),
			this, SLOT(processExperimentRecordCreateResponse(const Datastruct::ExperimentRecordCreateResponse &)));

		connect(SignalDispatch::instance(), SIGNAL(respQueryAllExperimentRecordResponse(const Datastruct::LoadAllExperimentRecordsResponse &)),
			this, SLOT(processQueryAllExperimentRecordResponse(const Datastruct::LoadAllExperimentRecordsResponse &)));

		connect(SignalDispatch::instance(), SIGNAL(respExperimentRecordDeleteResponse(const Datastruct::ExperimentRecordDeleteResponse &)),
			this, SLOT(processExperimentRecordDeleteResponse(const Datastruct::ExperimentRecordDeleteResponse &)));
	}

	void TrialSheetWidget::insertExperimentRecord()
	{
		Datastruct::ExperimentRecordCreateRequest request;
		request.id = Base::RUtil::UUID();
		request.taskId = m_taskId;
		request.lon = 0 ;
		request.lat = 0;
		request.seaCondition = QStringLiteral("true");
		request.platformId = Base::RUtil::UUID();
		QDateTime current_date_time = QDateTime::currentDateTime();
		request.floatingTime = current_date_time.toString("yyyy.MM.dd hh:mm:ss.zzz");

		NetConnector::instance()->write(request);
	}

	void TrialSheetWidget::deleteExperimentRecord(QString id)
	{
		Datastruct::ExperimentRecordDeleteRequest request;
		request.id = id;
		NetConnector::instance()->write(request);
	}

	void TrialSheetWidget::refreshCurrPage()
	{
		Datastruct::LoadAllExperimentRecordsRequest request;
		request.taskId = m_taskId;
		request.m_offsetIndex = m_pageSwitch->dataOffset();
		request.m_limitIndex = m_pageSwitch->perPageCount();
		NetConnector::instance()->write(request);
	}

}//namespace Related 