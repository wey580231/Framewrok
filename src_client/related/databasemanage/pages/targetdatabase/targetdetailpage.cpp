#include "targetdetailpage.h"

#include <QDebug>

#include "../../utils/util.h"
#include "../../customwidget/customwidgetcontainer.h"
#include "../../net/datanetconnector.h"
#include "../../net/signaldispatch.h"
#include "../../global.h"

namespace Related {

	TargetDetailPage::TargetDetailPage(QWidget *parent)
		: AbstractPage(parent),
		m_firstLoadData(true)
	{
		init();
		initConnect();
	}

	TargetDetailPage::~TargetDetailPage()
	{
	}

	PageType TargetDetailPage::getPageType() const
	{
		return Page_TargetDatabase_TargetDetail;
	}

	void TargetDetailPage::prepareBringToTop()
	{
		if (m_firstLoadData) {

			m_firstLoadData = false;
		}
	}

	void TargetDetailPage::init()
	{
		// ������Ϣ
		CustomWidgetContainer * cbwidget = new CustomWidgetContainer();
		{
			m_targetBaseInfoItem = new TargetBaseInfoItem();

			QWidget * bwidget = new QWidget();
			QVBoxLayout * blayout = new QVBoxLayout();
			blayout->setContentsMargins(0, 0, 0, 0);
			blayout->addWidget(m_targetBaseInfoItem);
			bwidget->setLayout(blayout);
			cbwidget->setContent(bwidget);
		}

		// �����Ϣ
		CustomWidgetContainer * ctabwidget = new CustomWidgetContainer();
		{
			m_tableView = new Base::RTableView();
			m_tableView->setFocusPolicy(Qt::NoFocus);
			m_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
			m_tableView->setSelectionMode(QAbstractItemView::SingleSelection);

			m_tableView = new Base::RTableView();
			m_tableView->setFocusPolicy(Qt::NoFocus);
			m_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
			m_tableView->setSelectionMode(QAbstractItemView::SingleSelection);

			m_tableModel = new TargetWavDataModel();
			m_tableView->setModel(m_tableModel);
			m_tableView->addColumnItem(Base::ColumnItem(TWAV_Index, QStringLiteral("����")));
			m_tableView->addColumnItem(Base::ColumnItem(TWAV_TargetName, QStringLiteral("¼��ʱ��"), 180));
			m_tableView->addColumnItem(Base::ColumnItem(TWAV_Edttime, QStringLiteral("����")));
			m_tableView->addColumnItem(Base::ColumnItem(TWAV_Tonnage, QStringLiteral("����")));
			m_tableView->addColumnItem(Base::ColumnItem(TWAV_Lon, QStringLiteral("����")));
			m_tableView->addColumnItem(Base::ColumnItem(TWAV_Lat, QStringLiteral("����")));
			m_tableView->addColumnItem(Base::ColumnItem(TWAV_Speed, QStringLiteral("�˸�")));
			m_tableView->addColumnItem(Base::ColumnItem(TWAV_AxlesNumber, QStringLiteral("����")));
			m_tableView->addColumnItem(Base::ColumnItem(TWAV_Type, QStringLiteral("����")));

			connect(m_tableView, SIGNAL(clicked(QModelIndex)), this, SLOT(slotClickedTable(QModelIndex)));

			m_pageSwitch = new PageSwitchBar();
			m_pageSwitch->setDataSize(m_tableModel->datasSize());
			connect(m_pageSwitch, SIGNAL(perPageNumsChanged(int)), this, SLOT(setFixedPageRowCount(int)));
			connect(m_pageSwitch, SIGNAL(switchPage(int)), this, SLOT(setPageNum(int)));

			QWidget * tbwidget = new QWidget();
			QVBoxLayout * tblayout = new QVBoxLayout();
			tblayout->setContentsMargins(0, 0, 0, 0);
			tblayout->addWidget(m_tableView);
			tblayout->addWidget(m_pageSwitch);
			tbwidget->setLayout(tblayout);
			ctabwidget->setContent(tbwidget);
		}

		// ͼƬ��Ϣ
		CustomWidgetContainer * ciwidget = new CustomWidgetContainer();
		{

		}

		QVBoxLayout * mianLayout = new QVBoxLayout();
		mianLayout->setContentsMargins(4, 4, 4, 4);
		mianLayout->addWidget(cbwidget);
		mianLayout->addWidget(ctabwidget);
		mianLayout->addWidget(ciwidget);
		setLayout(mianLayout);
	}

	void TargetDetailPage::initConnect()
	{
	}

} //namespace Related
