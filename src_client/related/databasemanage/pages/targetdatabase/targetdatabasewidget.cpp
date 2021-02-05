#include "targetdatabasewidget.h"

#include <QDebug>

#include "targetoverviewpage.h"
#include "targetdetailpage.h"

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

	void TargetDatabaseWidget::prepareBringToTop()
	{
		AbstractPage * cpage = dynamic_cast<AbstractPage *>(m_tabWidget->currentWidget());
		if (cpage)
			cpage->prepareBringToTop();
	}

	void TargetDatabaseWidget::init()
	{
		m_tabWidget = new Base::RTabWidget();
		m_tabWidget->setTabBarHeight(60);

		m_targetOverViewPage = new TargetOverViewPage();
		m_targetDetailPage = new TargetDetailPage();

		m_tabWidget->addPage(QStringLiteral("目标库概览"), m_targetOverViewPage);
		m_tabWidget->addPage(QStringLiteral("目标详情"), m_targetDetailPage);

		m_tabWidget->setWidgetBringToBottomCallback([&](QWidget * preWidet) {
			AbstractPage * preparePage = dynamic_cast<AbstractPage *>(preWidet);
			if (preparePage)
				preparePage->prepareBringToBottom();
		});

		m_tabWidget->setWidgetBringToTopCallback([&](QWidget * newWidet) {
			AbstractPage * nextPage = dynamic_cast<AbstractPage *>(newWidet);
			if (nextPage)
				nextPage->prepareBringToTop();
		});

		QVBoxLayout * layout = new QVBoxLayout();
		layout->addWidget(m_tabWidget);
		layout->setContentsMargins(4, 4, 4, 4);
		setLayout(layout);
	}

}//namespace Related 