#include "targetoverviewpage.h"

#include <QDebug>
#include <QComboBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QListView>

#include "../../utils/util.h"
#include "../../customwidget/customwidgetcontainer.h"
#include "../../net/datanetconnector.h"
#include "../../net/signaldispatch.h"

namespace Related {

	TargetOverViewPage::TargetOverViewPage(QWidget *parent)
		: AbstractPage(parent),
		m_firstLoadData(true)
	{
		init();
		initConnect();
	}

	TargetOverViewPage::~TargetOverViewPage()
	{
	}

	PageType TargetOverViewPage::getPageType() const
	{
		return Page_TargetDatabase_TargetOverview;
	}

	void TargetOverViewPage::prepareBringToTop()
	{
		if (m_firstLoadData) {
			refreshCurrPage();
			m_firstLoadData = false;
		}
	}

	void TargetOverViewPage::respToolButtPressed(OperationToolsPage::ButtType type)
	{
		switch (type)
		{
			case OperationToolsPage::Butt_Add: {
				createTargetDataInfo();
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

	void TargetOverViewPage::setPageNum(int page)
	{
	}

	void TargetOverViewPage::setFixedPageRowCount(int pageItemCount)
	{
		m_tableModel->setFixedPageRowCount(pageItemCount);
	}

	void TargetOverViewPage::slotTableDoubleClicked(const QModelIndex & index)
	{
		QString indexId;

		emit signalSelecteTargetDataInfo(indexId);
	}

	void TargetOverViewPage::processQueryTargetListResponse(const Datastruct::LoadAllTargetResponse & response)
	{
		m_tableModel->prepareData(response.m_targetInfos);
		m_pageSwitch->setDataSize(response.m_targetCount);
	}

	void TargetOverViewPage::processTargetCreateResponse(const Datastruct::TargetCreateResponse & response)
	{
		if (response.m_createResult) {
			refreshCurrPage();
		}
	}

	void TargetOverViewPage::processTargetDeleteResponse(const Datastruct::TargetDeleteResponse & response)
	{
		if (response.m_deleteResult) {
			refreshCurrPage();
		}
	}

	void TargetOverViewPage::processTargetModifyResponse(const Datastruct::TargetModifyResponse & response)
	{
		if (response.m_modifyResult) {
			refreshCurrPage();
		}
	}

	void TargetOverViewPage::init()
	{
		CustomWidgetContainer * cwidget = new CustomWidgetContainer();
		{
			m_operationToolsPage = new OperationToolsPage();
			connect(m_operationToolsPage, SIGNAL(buttPressed(OperationToolsPage::ButtType)),
				this, SLOT(respToolButtPressed(OperationToolsPage::ButtType)));

			//��ʱ���Դ���
			QWidget * conditionFilterWidget = new QWidget();
			QHBoxLayout * filterLayout = new QHBoxLayout();
			filterLayout->setContentsMargins(0, 0, 4, 0);
			conditionFilterWidget->setLayout(filterLayout);

			auto createLabel = [](QString text) {
				QLabel * label = new QLabel();
				label->setText(text);
				return label;
			};

			filterLayout->addWidget(createLabel(QStringLiteral("Ŀ������")));

			QComboBox * ct = new QComboBox();
			ct->setView(new QListView());
			QStringList ctList;
			ctList << QStringLiteral("ȫ������") << QStringLiteral("����") << QStringLiteral("����") << QStringLiteral("����");
			ct->addItems(ctList);

			filterLayout->addWidget(ct);
			filterLayout->addWidget(createLabel(QStringLiteral("����")));

			QComboBox * category = new QComboBox();
			category->setView(new QListView());
			QStringList cateList;
			cateList << QStringLiteral("ȫ������") << QStringLiteral("ˮ�潢") << QStringLiteral("�̴�") << QStringLiteral("�洬");
			category->addItems(cateList);
			
			filterLayout->addWidget(category);
			filterLayout->addWidget(createLabel(QStringLiteral("�ͺ�")));

			QComboBox * type = new QComboBox();
			type->setView(new QListView());
			QStringList tList;
			tList << QStringLiteral("ȫ���ͺ�") << QStringLiteral("XX��") << QStringLiteral("XXA��");
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
			m_tableModel = new TargetOverViewModel();

			m_tableView = new Base::RTableView();
			m_tableView->setFocusPolicy(Qt::NoFocus);
			m_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
			m_tableView->setSelectionMode(QAbstractItemView::SingleSelection);

			m_tableView->setModel(m_tableModel);
			m_tableView->addColumnItem(Base::ColumnItem(T_Index, QStringLiteral("����")));
			m_tableView->addColumnItem(Base::ColumnItem(T_TargetName, QStringLiteral("Ŀ������"), 140));
			m_tableView->addColumnItem(Base::ColumnItem(T_Edttime, QStringLiteral("¼��ʱ��"), 180));
			m_tableView->addColumnItem(Base::ColumnItem(T_Tonnage, QStringLiteral("��λ(t)"), 180));
			m_tableView->addColumnItem(Base::ColumnItem(T_Lon, QStringLiteral("����"), 180));
			m_tableView->addColumnItem(Base::ColumnItem(T_Lat, QStringLiteral("γ��"), 180));
			m_tableView->addColumnItem(Base::ColumnItem(T_Speed, QStringLiteral("�����ٶ�"), 180));
			m_tableView->addColumnItem(Base::ColumnItem(T_AxlesNumber, QStringLiteral("����")));
			m_tableView->addColumnItem(Base::ColumnItem(T_Type, QStringLiteral("����")));

			connect(m_tableView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(slotTableDoubleClicked(QModelIndex)));

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

	void TargetOverViewPage::initConnect()
	{
		connect(SignalDispatch::instance(), SIGNAL(respQueryAllTargetResponse(const Datastruct::LoadAllTargetResponse &)),
			this, SLOT(processQueryTargetListResponse(const Datastruct::LoadAllTargetResponse &)));

		connect(SignalDispatch::instance(), SIGNAL(respTargetCreateResponse(const Datastruct::TargetCreateResponse &)),
			this, SLOT(processTargetCreateResponse(const Datastruct::TargetCreateResponse &)));

		connect(SignalDispatch::instance(), SIGNAL(respTargetDeleteResponse(const Datastruct::TargetDeleteResponse &)),
			this, SLOT(processTargetDeleteResponse(const Datastruct::TargetDeleteResponse &)));

		connect(SignalDispatch::instance(), SIGNAL(respTargetModifyResponse(const Datastruct::TargetModifyResponse &)),
			this, SLOT(processTargetModifyResponse(const Datastruct::TargetModifyResponse &)));
	}

	void TargetOverViewPage::refreshCurrPage()
	{
		Datastruct::LoadAllTargetRequest request;
		request.m_offsetIndex = m_pageSwitch->dataOffset();
		request.m_limitIndex = m_pageSwitch->perPageCount();
		DataNetConnector::instance()->write(request);
	}

	void TargetOverViewPage::createTargetDataInfo()
	{
		QDateTime current_date_time = QDateTime::currentDateTime();

		Datastruct::TargetCreateRequest request;
		request.m_id = Base::RUtil::UUID();
		request.m_name = QStringLiteral("Ŀ��1");
		request.m_type = 1;
		request.m_creanTime = current_date_time.toString(TIME_FORMAT);
		DataNetConnector::instance()->write(request);
	}


}//namespace Related 
