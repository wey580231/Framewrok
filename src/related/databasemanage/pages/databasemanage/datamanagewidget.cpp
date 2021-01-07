#include "datamanagewidget.h"

#include <qboxlayout.h>

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

	void DataManageWidget::init()
	{
		m_tabWidget = new Base::RTabBar();
		m_tabWidget->setFixedHeight(75);
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

		QVBoxLayout * layout = new QVBoxLayout();
		layout->addWidget(m_tabWidget);
		layout->addWidget(m_stackedWidget);
		setLayout(layout);
	}

} //namespace Related 