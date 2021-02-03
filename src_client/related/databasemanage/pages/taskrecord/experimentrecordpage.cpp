#include "experimentrecordpage.h"

#include <QDebug>

#include "../../utils/util.h"
#include "../../customwidget/customwidgetcontainer.h"
#include "../../net/datanetconnector.h"
#include "../../net/signaldispatch.h"
#include "../../global.h"

namespace Related {

	ExperimentRecordPage::ExperimentRecordPage(QWidget *parent)
		: AbstractPage(parent),
		m_operationToolsPage(nullptr),
		m_tableView(nullptr),
		m_tableModel(nullptr),
		m_pageSwitch(nullptr),
		m_firstLoadData(true),
		m_seleteTableRow(EXPERIMENT_RECORD_SELET_MAX_INDEX)
	{
		init();
		initConnent();
	}

	ExperimentRecordPage::~ExperimentRecordPage()
	{
		if (m_operationToolsPage != nullptr) {
			delete m_operationToolsPage;
			m_operationToolsPage = nullptr;
		}
		if (m_tableView != nullptr) {
			delete m_tableView;
			m_tableView = nullptr;
		}
		if (m_tableModel != nullptr) {
			delete m_tableModel;
			m_tableModel = nullptr;
		}
		if (m_pageSwitch != nullptr) {
			delete m_pageSwitch;
			m_pageSwitch = nullptr;
		}
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
			ExperimentRecordEditDialog editDialog(this);
			editDialog.setExperimentRecordDataOperatioType(ExperimentRecordEditDialog::ERD_Create);
			editDialog.setTaskId(m_taskId);
			if (QDialog::Accepted == editDialog.exec()) {
				refreshCurrPage();
				m_seleteTableRow = EXPERIMENT_RECORD_SELET_MAX_INDEX;
			}
		}
			break;

		case OperationToolsPage::Butt_Delete: {
			if (m_seleteTableRow <  m_allExperimentRecords.m_experimentRecordInfos.size()
				&& m_seleteTableRow > EXPERIMENT_RECORD_SELET_MAX_INDEX) {
				int result = Base::RMessageBox::information(this, 
					QStringLiteral("提示"), QStringLiteral("是否删除数据?"), 
					Base::RMessageBox::Yes | Base::RMessageBox::No);
				if (result != Base::RMessageBox::Yes) {
					return;
				}

				Datastruct::ExperimentRecordEntityData data = m_allExperimentRecords.m_experimentRecordInfos.at(m_seleteTableRow);
				deleteExperimentRecord(data.id);
				m_seleteTableRow = EXPERIMENT_RECORD_SELET_MAX_INDEX;
			}
		}
			break;

		case OperationToolsPage::Butt_Edit: {

			if (m_seleteTableRow < m_allExperimentRecords.m_experimentRecordInfos.size() 
				&& m_seleteTableRow > EXPERIMENT_RECORD_SELET_MAX_INDEX) {
				Datastruct::ExperimentRecordEntityData data = m_allExperimentRecords.m_experimentRecordInfos.at(m_seleteTableRow);
				
				ExperimentRecordEditDialog editDialog(this);
				editDialog.setExperimentRecordDataOperatioType(ExperimentRecordEditDialog::ERD_Modify);
				editDialog.setExperimentRecordEntityData(data);
				if (QDialog::Accepted == editDialog.exec()) {
					refreshCurrPage();
					m_seleteTableRow = EXPERIMENT_RECORD_SELET_MAX_INDEX;
				}
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

	void ExperimentRecordPage::setPageNum(int page)
	{
		refreshCurrPage();
	}

	void ExperimentRecordPage::setFixedPageRowCount(int pageItemCount)
	{
		m_tableModel->setFixedPageRowCount(pageItemCount);
	}

	void ExperimentRecordPage::processQueryAllExperimentRecordResponse(const Datastruct::LoadAllExperimentRecordsResponse & response)
	{
		m_allExperimentRecords = response;
		m_tableModel->prepareData(response.m_experimentRecordInfos);
		m_pageSwitch->setDataSize(response.m_experimentRecordCount);
	}

	void ExperimentRecordPage::processExperimentRecordDeleteResponse(const Datastruct::ExperimentRecordDeleteResponse & response)
	{
		if (response.m_deleteResult)
		{
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
			m_tableView->addColumnItem(Base::ColumnItem(ER_Index, QStringLiteral("索引")));
			m_tableView->addColumnItem(Base::ColumnItem(ER_PlatformId, QStringLiteral("平台名"), 140));
			m_tableView->addColumnItem(Base::ColumnItem(ER_FloatingTime, QStringLiteral("上浮时间")));
			m_tableView->addColumnItem(Base::ColumnItem(ER_Lon, QStringLiteral("上浮经度")));
			m_tableView->addColumnItem(Base::ColumnItem(ER_Lat, QStringLiteral("上浮纬度")));
			m_tableView->addColumnItem(Base::ColumnItem(ER_SetHeadingDegree, QStringLiteral("设置航向角")));

			m_tableView->addColumnItem(Base::ColumnItem(ER_ActualHeadingDegree, QStringLiteral("实际航向角")));
			m_tableView->addColumnItem(Base::ColumnItem(ER_AcousticState, QStringLiteral("声学系统状态")));
			m_tableView->addColumnItem(Base::ColumnItem(ER_TargetNum, QStringLiteral("目标总数")));
			m_tableView->addColumnItem(Base::ColumnItem(ER_UnderwaterTargetNum, QStringLiteral("水下目标数")));
			m_tableView->addColumnItem(Base::ColumnItem(ER_UnderwaterTargetInfo, QStringLiteral("水下目标信息")));
			m_tableView->addColumnItem(Base::ColumnItem(ER_MaxDepth, QStringLiteral("剖面最大深度")));
			m_tableView->addColumnItem(Base::ColumnItem(ER_ProfileIndex, QStringLiteral("剖面序号")));
			m_tableView->addColumnItem(Base::ColumnItem(ER_ProfileLength, QStringLiteral("剖面时长")));
			m_tableView->addColumnItem(Base::ColumnItem(ER_ProfileDistance, QStringLiteral("剖面移动距离")));

			connect(m_tableView, SIGNAL(clicked(QModelIndex)), this, SLOT(slotClickedTable(QModelIndex)));

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

	void ExperimentRecordPage::initConnent()
	{
		connect(SignalDispatch::instance(), SIGNAL(respQueryAllExperimentRecordResponse(const Datastruct::LoadAllExperimentRecordsResponse &)),
			this, SLOT(processQueryAllExperimentRecordResponse(const Datastruct::LoadAllExperimentRecordsResponse &)));

		connect(SignalDispatch::instance(), SIGNAL(respExperimentRecordDeleteResponse(const Datastruct::ExperimentRecordDeleteResponse &)),
			this, SLOT(processExperimentRecordDeleteResponse(const Datastruct::ExperimentRecordDeleteResponse &)));
	}

	void ExperimentRecordPage::deleteExperimentRecord(QString id)
	{
		Datastruct::ExperimentRecordDeleteRequest request;
		request.m_id = id;
		DataNetConnector::instance()->write(request);
	}

	void ExperimentRecordPage::refreshCurrPage()
	{
		Datastruct::LoadAllExperimentRecordsRequest request;
		request.m_taskId = m_taskId;
		request.m_offsetIndex = m_pageSwitch->dataOffset();
		request.m_limitIndex = m_pageSwitch->perPageCount();
		DataNetConnector::instance()->write(request);
	}

}//namespace Related 