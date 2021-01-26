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

	void TargetDatabaseWidget::init()
	{
		m_tabWidget = new Base::RTabWidget();
		m_tabWidget->setTabBarHeight(60);

		m_targetDatabaseManagePage = new TargetDatabaseManagePage();

		m_tabWidget->addPage(QStringLiteral("Ä¿±ê¿â"), m_targetDatabaseManagePage);

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
		layout->setContentsMargins(0, 0, 0, 0);
		setLayout(layout);
	}

}//namespace Related 