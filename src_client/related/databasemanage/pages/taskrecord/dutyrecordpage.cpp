#include "dutyrecordpage.h"

#include <QDebug>

#include "../../utils/util.h"
#include "../../customwidget/customwidgetcontainer.h"
#include "../../net/netconnector.h"
#include "../../net/signaldispatch.h"
#include "../../global.h"

namespace Related {

	DutyRecordPage::DutyRecordPage(QWidget *parent)
		: AbstractPage(parent),m_firstLoadData(true)
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

	void DutyRecordPage::processDutyRecordCreateResponse(const Datastruct::DutyRecordCreateResponse & response)
	{
		if (response.m_createResult == true) {
			refreshCurrPage();
		}
	}

	void DutyRecordPage::processQueryAllDutyRecordResponse(const Datastruct::LoadAllDutyRecordResponse & response)
	{
		m_tableModel->prepareData(response.m_dutyRecordInfos);
		m_pageSwitch->setDataSize(response.m_dutyRecordCount);
	}

	void DutyRecordPage::processDutyRecordDeleteResponse(const Datastruct::DutyRecordDeleteResponse & response)
	{
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
			m_tableView = new Base::RTableView();
			m_tableView->setFocusPolicy(Qt::NoFocus);
			m_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
			m_tableView->setSelectionMode(QAbstractItemView::SingleSelection);

			m_tableModel = new LogbookModel();

			m_tableView->setModel(m_tableModel);
			m_tableView->addColumnItem(Base::ColumnItem(L_Index, QStringLiteral("索引")));
			m_tableView->addColumnItem(Base::ColumnItem(L_CreateTime, QStringLiteral("录入时间"), 180));
			m_tableView->addColumnItem(Base::ColumnItem(L_Description, QStringLiteral("描述")));
			m_tableView->addColumnItem(Base::ColumnItem(L_SeaCondition, QStringLiteral("海况信息")));

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
	}

	/*!
	 * @brief   插入一条记录
	 * @details 
	 */
	void DutyRecordPage::insertDutyRecord()
	{
		Datastruct::DutyRecordCreateRequest request;
		request.id = Base::RUtil::UUID();
		request.taskId = m_taskId;
		QDateTime current_date_time = QDateTime::currentDateTime();
		request.createTime = current_date_time.toString("yyyy.MM.dd hh:mm:ss.zzz");
		request.description = QStringLiteral("1");
		request.seaCondition = QStringLiteral("1");
		NetConnector::instance()->write(request);
	}

	void DutyRecordPage::refreshCurrPage()
	{
		Datastruct::LoadAllDutyRecordRequest request;
		request.taskId = m_taskId;
		request.m_offsetIndex = m_pageSwitch->dataOffset();
		request.m_limitIndex = m_pageSwitch->perPageCount();
		NetConnector::instance()->write(request);
	}


}//namespace Related