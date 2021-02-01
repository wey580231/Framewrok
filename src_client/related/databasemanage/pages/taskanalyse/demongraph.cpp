#include "demongraph.h"

#include <QHBoxLayout>
#include <QGridLayout>

#include "../../customwidget/rcustomplot.h"
#include "../../customwidget/tinywavgraph.h"

namespace Related {

	DemonGraph::DemonGraph(QWidget *parent)
		: AbstractPage(parent)
	{
		init();
	}

	DemonGraph::~DemonGraph()
	{

	}

	PageType DemonGraph::getPageType() const
	{
		return Page_TaskAnalyse_DomenAnalysis;
	}

	void DemonGraph::init()
	{
		QWidget * mainWidget = new QWidget();

		m_tinyWav = new TinyWavGraph();
		m_tinyWav->setFixedHeight(35);

		m_mainGraph = new RCustomPlot();

		QWidget * graphWidget = new QWidget();
		QGridLayout * graphLayout = new QGridLayout();

		for (int i = 0; i < 4; i++) {
			RCustomPlot * ga = new RCustomPlot();
			RCustomPlot * gb = new RCustomPlot();

			graphLayout->addWidget(ga, i, 0, 1, 1);
			graphLayout->addWidget(gb, i, 1, 1, 1);
		}

		graphWidget->setLayout(graphLayout);

		QHBoxLayout * mainLayout = new QHBoxLayout();
		mainLayout->setContentsMargins(0, 0, 0, 0);
		mainLayout->addWidget(m_mainGraph);
		mainLayout->addWidget(graphWidget);
		mainWidget->setLayout(mainLayout);

		QVBoxLayout * layout = new QVBoxLayout();
		layout->setContentsMargins(4, 4, 4, 4);
		layout->addWidget(m_tinyWav);
		layout->addWidget(mainWidget);
		this->setLayout(layout);
	}

} //namespace Related 