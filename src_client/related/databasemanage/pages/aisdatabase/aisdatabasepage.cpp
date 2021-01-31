#include "aisdatabasepage.h"

#include <QDebug>

namespace Related {

	AisDataBasePage::AisDataBasePage(QWidget *parent)
		: AbstractPage(parent)
	{
		init();
	}

	AisDataBasePage::~AisDataBasePage()
	{
	}

	PageType AisDataBasePage::getPageType() const
	{
		return Page_AisDataBase;
	}

	void AisDataBasePage::prepareBringToTop()
	{
		AbstractPage * cpage = dynamic_cast<AbstractPage *>(m_tabWidget->currentWidget());
		if (cpage)
			cpage->prepareBringToTop();
	}

	void AisDataBasePage::init()
	{
		m_tabWidget = new Base::RTabWidget();
		m_tabWidget->setTabBarHeight(60);

		m_aisDataPage = new AisDataPage();

		m_tabWidget->addPage(QStringLiteral("AISÊý¾Ý"), m_aisDataPage);

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

} //namespace Related {