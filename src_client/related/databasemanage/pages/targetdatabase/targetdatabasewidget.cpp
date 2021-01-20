#include "targetdatabasewidget.h"

#include <QDebug>

namespace Related {

	TargetDatabaseWidget::TargetDatabaseWidget(QWidget *parent)
		: AbstractPage(parent)
	{
		init();
	}

	TargetDatabaseWidget::~TargetDatabaseWidget()
	{
	}

	Related::PageType TargetDatabaseWidget::getPageType() const
	{
		return Page_TargetDatabase;
	}

	void TargetDatabaseWidget::respTabChanged(int page)
	{
		m_stackedWidget->setCurrentIndex(page);
	}

	void TargetDatabaseWidget::init()
	{
		m_tabWidget = new Base::RTabBar();
		connect(m_tabWidget, SIGNAL(currentIndexChanged(int)), this, SLOT(respTabChanged(int)));

		m_tabWidget->setFixedHeight(60);
		m_tabWidget->setTabAlignment(Base::RTabBar::AlignLeft);

		Base::RTabButton * target1Butt = new Base::RTabButton(QStringLiteral("目标库"));
		//Base::RTabButton * target2Butt = new Base::RTabButton(QStringLiteral("目标库管理2"));

		m_tabWidget->addTabButton(target1Butt, Tab_target1);
		//m_tabWidget->addTabButton(target2Butt, Tab_target2);
		m_tabWidget->setFixedHeight(60);

		m_stackedWidget = new QStackedWidget();

		m_targetDatabaseManagePage = new TargetDatabaseManagePage();

		m_stackedWidget->addWidget(m_targetDatabaseManagePage);

		QVBoxLayout * layout = new QVBoxLayout();
		layout->addWidget(m_tabWidget);
		layout->addWidget(m_stackedWidget);
		layout->setContentsMargins(0, 0, 0, 0);
		setLayout(layout);
	}

}//namespace Related 