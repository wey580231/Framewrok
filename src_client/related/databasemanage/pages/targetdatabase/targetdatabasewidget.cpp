#include "targetdatabasewidget.h"

#include <QDebug>

#include "targetoverviewpage.h"
#include "targetdetailpage.h"


#include "../taskanalyse/timedomainanalyse.h"
#include "../taskanalyse/dynamicgraphanalysis.h"
#include "../taskanalyse/demongraph.h"
#include "../taskanalyse/lofargraph.h"
#include "../taskanalyse/situationanalysis.h"

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

	void TargetDatabaseWidget::slotSelecteTargetDataInfo(QString indexId)
	{
		m_tabWidget->setSwitchWidget(m_targetDetailPage);
	}

	void TargetDatabaseWidget::init()
	{
		m_tabWidget = new Base::RTabWidget();
		m_tabWidget->setTabBarHeight(60);

		m_targetOverViewPage = new TargetOverViewPage();
		connect(m_targetOverViewPage, SIGNAL(signalSelecteTargetDataInfo(QString)),
			this, SLOT(slotSelecteTargetDataInfo(QString)));

		m_targetDetailPage = new TargetDetailPage(this);
		m_timeAnalyseWidget = new TimeDomainAnalyse(this);
		m_audioDataWidget = new DynamicGraphAnalysis(this);
		m_demonAnalysis = new DemonGraph(this);
		m_lofarAnalysis = new LofarGraph(this);
		m_situationAnalysis = new SituationAnalysis(this);

		m_tabWidget->addPage(QStringLiteral("目标库概览"), m_targetOverViewPage);
		m_tabWidget->addPage(QStringLiteral("目标详情"), m_targetDetailPage);
		m_tabWidget->addPage(QStringLiteral("时域分析"), m_timeAnalyseWidget);
		m_tabWidget->addPage(QStringLiteral("频域分析"), m_audioDataWidget);
		m_tabWidget->addPage(QStringLiteral("DEMON分析"), m_demonAnalysis);
		m_tabWidget->addPage(QStringLiteral("LOFAR分析"), m_lofarAnalysis);
		m_tabWidget->addPage(QStringLiteral("态势分析"), m_situationAnalysis);

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