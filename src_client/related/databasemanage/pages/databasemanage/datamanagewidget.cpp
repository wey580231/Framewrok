#include "datamanagewidget.h"

#include <qboxlayout.h>
#include <QHeaderView>

namespace Related {

	DataManageWidget::DataManageWidget(QWidget *parent)
		: AbstractPage(parent)
	{
		init();
	}

	DataManageWidget::~DataManageWidget()
	{
	}

	Related::PageType DataManageWidget::getPageType() const
	{
		return Page_DataManage;
	}

	void DataManageWidget::respTabChanged(int page)
	{
		m_stackedWidget->setCurrentIndex(page);
	}

	void DataManageWidget::init()
	{
		m_tabWidget = new Base::RTabBar();
		connect(m_tabWidget, SIGNAL(currentIndexChanged(int)), this, SLOT(respTabChanged(int)));
		m_tabWidget->setFixedHeight(60);
		
		m_tabWidget->setTabAlignment(Base::RTabBar::AlignLeft);

		Base::RTabButton * level0Butt = new Base::RTabButton(QStringLiteral("0级库"));
		Base::RTabButton * level1Butt = new Base::RTabButton(QStringLiteral("1级库"));
		Base::RTabButton * level2Butt = new Base::RTabButton(QStringLiteral("2级库"));
		Base::RTabButton * otherButt = new Base::RTabButton(QStringLiteral("其它库"));

		m_tabWidget->addTabButton(level0Butt, Tab_level0);
		m_tabWidget->addTabButton(level1Butt, Tab_level1);
		m_tabWidget->addTabButton(level2Butt, Tab_level2);
		m_tabWidget->addTabButton(otherButt, Tab_other);

		m_stackedWidget = new QStackedWidget();
		m_stackedWidget->setStyleSheet("background-color:rgba(0,0,0,0)");

		m_pageLevel0 = new Level0Page(this);
		m_pageLevel1 = new Level1Page(this);
		m_pageLevel2 = new Level2Page(this);

		m_stackedWidget->addWidget(m_pageLevel0);
		m_stackedWidget->addWidget(m_pageLevel1);
		m_stackedWidget->addWidget(m_pageLevel2);

		QVBoxLayout * layout = new QVBoxLayout();
		layout->setContentsMargins(0, 0, 0, 0);
		layout->addWidget(m_tabWidget);
		layout->addWidget(m_stackedWidget);
		setLayout(layout);
	}

} //namespace Related 