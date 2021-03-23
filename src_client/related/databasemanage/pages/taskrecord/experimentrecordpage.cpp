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
			ExperimentRecordEditDialog editDialog(m_taskId, ExperimentRecordEditDialog::ERD_Create, this);
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
					QStringLiteral("��ʾ"), QStringLiteral("�Ƿ�ɾ������?"), 
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
				
				ExperimentRecordEditDialog editDialog(m_taskId, ExperimentRecordEditDialog::ERD_Modify, this);
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

	void ExperimentRecordPage::slotDetectPlatformCurrentIndexChanged(int index)
	{
		refreshCurrPage();
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
			
			QLabel * detectPlatformLabel = new QLabel();
			detectPlatformLabel->setText(QStringLiteral("���ƽ̨��"));

			m_detectPlatformComboBox = new QComboBox();
			QStringList strList;
			strList << QStringLiteral("����ƽ̨") 
				<< QStringLiteral("HXJ01")
				<< QStringLiteral("HXJ02") 
				<< QStringLiteral("HXJ03")
				<< QStringLiteral("HXJ04");
			m_detectPlatformComboBox->addItems(strList);
			connect(m_detectPlatformComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(slotDetectPlatformCurrentIndexChanged(int)));

			QWidget * widget = new QWidget();
			QHBoxLayout * layout = new QHBoxLayout();
			layout->addWidget(m_operationToolsPage);
			layout->addWidget(detectPlatformLabel);
			layout->addWidget(m_detectPlatformComboBox);
			layout->setContentsMargins(0,0,0,0);
			widget->setLayout(layout);
			cwidget->setContent(widget);
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
		if (m_detectPlatformComboBox->currentIndex() != 0) {
			request.m_platformId = m_detectPlatformComboBox->currentText();
		}
		request.m_offsetIndex = m_pageSwitch->dataOffset();
		request.m_limitIndex = m_pageSwitch->perPageCount();
		DataNetConnector::instance()->write(request);
	}

}//namespace Related 