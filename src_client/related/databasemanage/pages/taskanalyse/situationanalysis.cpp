#include "situationanalysis.h"

#include <QHBoxLayout>
#include <QVBoxLayout>

#include "../../customwidget/rcustomplot.h"

namespace Related {

	SituationAnalysis::SituationAnalysis(QWidget *parent)
		: AbstractPage(parent)
	{
		init();
	}

	SituationAnalysis::~SituationAnalysis()
	{
	}

	Related::PageType SituationAnalysis::getPageType() const
	{
		return Page_TaskAnalyse_SituationAnalysi;
	}

	void SituationAnalysis::init()
	{
		QWidget * mainWidget = new QWidget();

		m_mainGraph = new RCustomPlot();

		m_mapWidget = new QWidget();
		QLabel * label = new QLabel();
		label->setText(QStringLiteral("µç×ÓµØÍ¼"));

		QHBoxLayout * mapLayout = new QHBoxLayout();
		mapLayout->addWidget(label);
		m_mapWidget->setLayout(mapLayout);

		QHBoxLayout * mainLayout = new QHBoxLayout();
		mainLayout->setContentsMargins(0, 0, 0, 0);
		mainLayout->addWidget(m_mainGraph);
		mainLayout->addWidget(m_mapWidget);
		mainWidget->setLayout(mainLayout);

		QHBoxLayout * layout = new QHBoxLayout();
		layout->setContentsMargins(4, 4, 4, 4);
		layout->addWidget(mainWidget);
		this->setLayout(layout);
	}

} //namespace Related 