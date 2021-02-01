#include "dynamicgraphanalysis.h"

#include <QGridLayout>

#include "../../customwidget/rcustomplot.h"
#include "../../customwidget/tinywavgraph.h"

namespace Related {

	DynamicGraphAnalysis::DynamicGraphAnalysis(QWidget *parent)
		: AbstractPage(parent)
	{
		init();
	}

	DynamicGraphAnalysis::~DynamicGraphAnalysis()
	{

	}

	Related::PageType DynamicGraphAnalysis::getPageType() const
	{
		return Page_TaskAnalyse_FrequencyDomainAnalysis;
	}

	void DynamicGraphAnalysis::init()
	{
		QWidget * mainWidget = new QWidget();

		m_tinyWav = new TinyWavGraph();
		m_tinyWav->setFixedHeight(35);

		m_demoLine = new RCustomPlot();
		m_demoGraph = new RCustomPlot();
		m_lofarLine = new RCustomPlot();
		m_lofarGraph = new RCustomPlot();

		QVBoxLayout * mainLayout = new QVBoxLayout();
		mainLayout->setContentsMargins(0, 0, 0, 0);
		mainLayout->setSpacing(4);
		mainLayout->addWidget(m_tinyWav);
		mainLayout->addWidget(m_demoLine);
		mainLayout->addWidget(m_demoGraph);
		mainLayout->addWidget(m_lofarLine);
		mainLayout->addWidget(m_lofarGraph);
		mainWidget->setLayout(mainLayout);

		QHBoxLayout  * layout = new QHBoxLayout();
		layout->setContentsMargins(4, 4, 4, 4);
		layout->addWidget(mainWidget);
		this->setLayout(layout);
	}

	void DynamicGraphAnalysis::initPlot(QCustomPlot * plot)
	{

	}

}//namespace Related 