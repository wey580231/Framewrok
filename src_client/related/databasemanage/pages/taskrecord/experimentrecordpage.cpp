#include "experimentrecordpage.h"

#include <QDebug>

#include "../utils/util.h"
#include "../customwidget/customwidgetcontainer.h"
#include "../net/netconnector.h"
#include "../net/signaldispatch.h"
#include "../global.h"

namespace Related {

	ExperimentRecordPage::ExperimentRecordPage(QWidget *parent)
		: AbstractPage(parent),
		m_operationToolsPage(nullptr),
		m_tableView(nullptr),
		m_tableModel(nullptr),
		m_pageSwitch(nullptr),
		m_firstLoadData(true),
		m_seleteTableRow(99999999)
	{
		init();
		initConnent();
	}

	ExperimentRecordPage::~ExperimentRecordPage()
	{
	}

	PageType ExperimentRecordPage::getPageType() const
	{
		return Page_TaskRecord_ExperimentRecord;
	}

	void ExperimentRecordPage::prepareBringToTop()
	{
		if (m_firstLoadData) {
			refreshCurrPage();
			m_firstLoadData = false;
		}
	}

	void  ExperimentRecordPage::setTaskId(QString taskId)
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

	void ExperimentRecordPage::respToolButtPressed(OperationToolsPage::ButtType type) 
	{
		switch (type)
		{
		case OperationToolsPage::Butt_Add: {
			insertExperimentRecord();
		}
			break;

		case OperationToolsPage::Butt_Delete: {
			if (m_seleteTableRow < m_allExperimentRecords.m_experimentRecordInfos.size()) {
				Datastruct::ExperimentRecordEntityData data = m_allExperimentRecords.m_experimentRecordInfos.at(m_seleteTableRow);
				deleteExperimentRecord(data.id);
				m_seleteTableRow = 99999999;
			}
		}
			break;

		case OperationToolsPage::Butt_Edit: {

			if (m_seleteTableRow < m_allExperimentRecords.m_experimentRecordInfos.size()) {
				Datastruct::ExperimentRecordEntityData data = m_allExperimentRecords.m_experimentRecordInfos.at(m_seleteTableRow);
				
				data.lon = 100;
				data.lat = 100;
				data.setHeadingDegree = 100;
				data.maxDepth = 100;
				data.profileIndex = 100;
				data.profileLength = 100;
				
				modifyExperimentRecord(data);
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

	void ExperimentRecordPage::processExperimentRecordCreateResponse(const Datastruct::ExperimentRecordCreateResponse & response)
	{
		if (response.m_createResult == true) {
			refreshCurrPage();
		}
	}

	void ExperimentRecordPage::processQueryAllExperimentRecordResponse(const Datastruct::LoadAllExperimentRecordsResponse & response)
	{
		m_allExperimentRecords = response;
		m_tableModel->prepareData(response.m_experimentRecordInfos);
		m_pageSwitch->setDataSize(response.m_experimentRecordCount);
	}

	void ExperimentRecordPage::processExperimentRecordDeleteResponse(const Datastruct::ExperimentRecordDeleteResponse & response)
	{
		if (response.m_deleteResult) {
			refreshCurrPage();
		}
	}

	void ExperimentRecordPage::processExperimentRecordModifyResponse(const Datastruct::ExperimentRecordModifyResponse & response)
	{
		if (response.m_modifyResult) {
			refreshCurrPage();
		}
	}

	void ExperimentRecordPage::slotClickedTable(QModelIndex index)
	{
		m_seleteTableRow = index.row();
	}

	void ExperimentRecordPage::init()
	{
		CustomWidgetContainer * cwidget = new CustomWidgetContainer();
		{
			m_operationToolsPage = new OperationToolsPage();
			connect(m_operationToolsPage, SIGNAL(buttPressed(OperationToolsPage::ButtType)), this, SLOT(respToolButtPressed(OperationToolsPage::ButtType)));
			cwidget->setContent(m_operationToolsPage);
		}

		CustomWidgetContainer * ctableView = new CustomWidgetContainer();
		{
			m_tableModel = new ExperimentRecordModel();

			m_tableView = new Base::RTableView();
			m_tableView->setFocusPolicy(Qt::NoFocus);
			m_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
			m_tableView->setSelectionMode(QAbstractItemView::SingleSelection);
		
			m_tableView->setModel(m_tableModel);
			m_tableView->addColumnItem(Base::ColumnItem(ER_Index, QStringLiteral("����")));
			m_tableView->addColumnItem(Base::ColumnItem(ER_PlatformId, QStringLiteral("ƽ̨��"), 140));
			m_tableView->addColumnItem(Base::ColumnItem(ER_FloatingTime, QStringLiteral("�ϸ�ʱ��")));
			m_tableView->addColumnItem(Base::ColumnItem(ER_Lon, QStringLiteral("�ϸ�����")));
			m_tableView->addColumnItem(Base::ColumnItem(ER_Lat, QStringLiteral("�ϸ�γ��")));
			m_tableView->addColumnItem(Base::ColumnItem(ER_SetHeadingDegree, QStringLiteral("���ú����")));

			m_tableView->addColumnItem(Base::ColumnItem(ER_ActualHeadingDegree, QStringLiteral("ʵ�ʺ����")));
			m_tableView->addColumnItem(Base::ColumnItem(ER_AcousticState, QStringLiteral("��ѧϵͳ״̬")));
			m_tableView->addColumnItem(Base::ColumnItem(ER_TargetNum, QStringLiteral("Ŀ������")));
			m_tableView->addColumnItem(Base::ColumnItem(ER_UnderwaterTargetNum, QStringLiteral("ˮ��Ŀ����")));
			m_tableView->addColumnItem(Base::ColumnItem(ER_UnderwaterTargetInfo, QStringLiteral("ˮ��Ŀ����Ϣ")));
			m_tableView->addColumnItem(Base::ColumnItem(ER_MaxDepth, QStringLiteral("����������")));
			m_tableView->addColumnItem(Base::ColumnItem(ER_ProfileIndex, QStringLiteral("�������")));
			m_tableView->addColumnItem(Base::ColumnItem(ER_ProfileLength, QStringLiteral("����ʱ��")));
			m_tableView->addColumnItem(Base::ColumnItem(ER_ProfileDistance, QStringLiteral("�����ƶ�����")));

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

	void ExperimentRecordPage::initConnent()
	{
		connect(SignalDispatch::instance(), SIGNAL(respExperimentRecordCreateResponse(const Datastruct::ExperimentRecordCreateResponse &)),
			this, SLOT(processExperimentRecordCreateResponse(const Datastruct::ExperimentRecordCreateResponse &)));

		connect(SignalDispatch::instance(), SIGNAL(respQueryAllExperimentRecordResponse(const Datastruct::LoadAllExperimentRecordsResponse &)),
			this, SLOT(processQueryAllExperimentRecordResponse(const Datastruct::LoadAllExperimentRecordsResponse &)));

		connect(SignalDispatch::instance(), SIGNAL(respExperimentRecordDeleteResponse(const Datastruct::ExperimentRecordDeleteResponse &)),
			this, SLOT(processExperimentRecordDeleteResponse(const Datastruct::ExperimentRecordDeleteResponse &)));
		
		connect(SignalDispatch::instance(), SIGNAL(respExperimentRecordModifyResponse(const Datastruct::ExperimentRecordModifyResponse &)),
			this, SLOT(processExperimentRecordModifyResponse(const Datastruct::ExperimentRecordModifyResponse &)));

	}

	void ExperimentRecordPage::insertExperimentRecord()
	{
		QDateTime current_date_time = QDateTime::currentDateTime();

		Datastruct::ExperimentRecordCreateRequest request;
		request.m_id = Base::RUtil::UUID();						
		request.m_taskId = m_taskId;						
		request.m_platformId = QStringLiteral("platform01");
		request.m_floatingTime = current_date_time.toString("yyyy.MM.dd hh:mm:ss.zzz");
		request.m_lon = 0;							
		request.m_lat = 0;							
		request.m_setHeadingDegree = 0;				
		request.m_actualHeadingDegree = 0;			
		request.m_acousticState = 0;					
		request.m_targetNum = 1;						
		request.m_underwaterTargetNum = 1;				
		request.m_underwaterTargetInfo = QStringLiteral("0101");
		request.m_maxDepth = 0;						
		request.m_profileIndex = 1;						
		request.m_profileLength = 1;					
		request.m_profileDistance = 1;				

		NetConnector::instance()->write(request);
	}

	void ExperimentRecordPage::deleteExperimentRecord(QString id)
	{
		Datastruct::ExperimentRecordDeleteRequest request;
		request.m_id = id;
		NetConnector::instance()->write(request);
	}

	void ExperimentRecordPage::modifyExperimentRecord(Datastruct::ExperimentRecordEntityData data)
	{
		Datastruct::ExperimentRecordModifyRequest request;
		request.m_id = data.id;
		request.m_taskId = data.taskId;
		request.m_platformId = data.platformId;
		request.m_floatingTime = data.floatingTime;
		request.m_lon = data.lon;
		request.m_lat = data.lat;
		request.m_setHeadingDegree = data.setHeadingDegree;
		request.m_actualHeadingDegree = data.actualHeadingDegree;
		request.m_acousticState = data.acousticState;
		request.m_targetNum = data.targetNum;
		request.m_underwaterTargetNum = data.underwaterTargetNum;
		request.m_underwaterTargetInfo = data.underwaterTargetInfo;
		request.m_maxDepth = data.maxDepth;
		request.m_profileIndex = data.profileIndex;
		request.m_profileLength = data.profileLength;
		request.m_profileDistance = data.profileDistance;

		NetConnector::instance()->write(request);
	}

	void ExperimentRecordPage::refreshCurrPage()
	{
		Datastruct::LoadAllExperimentRecordsRequest request;
		request.m_taskId = m_taskId;
		request.m_offsetIndex = m_pageSwitch->dataOffset();
		request.m_limitIndex = m_pageSwitch->perPageCount();
		NetConnector::instance()->write(request);
	}

}//namespace Related 