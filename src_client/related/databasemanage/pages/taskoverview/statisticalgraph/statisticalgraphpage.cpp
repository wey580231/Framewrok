#include "statisticalgraphpage.h"

namespace Related {

	StatisticalGraphPage::StatisticalGraphPage(QWidget *parent)
		: QWidget(parent)
	{
		init();
	}

	StatisticalGraphPage::~StatisticalGraphPage()
	{

	}

	void StatisticalGraphPage::init()
	{
		QPieSlice *slice_1 = new QPieSlice(QStringLiteral("1ƽ̨"), 0.6);
		slice_1->setBrush(Qt::green);
		QPieSlice *slice_2 = new QPieSlice(QStringLiteral("2ƽ̨"), 0.4);
		slice_2->setBrush(Qt::blue);

		QPieSeries *series = new QPieSeries();
		series->append(slice_1);
		series->append(slice_2);

		m_chart = new QChart();
		m_chart->addSeries(series);
		//chart->setAnimationOptions(QChart::AllAnimations); 

		m_chart->setMaximumHeight(300);

		m_chartView = new QChartView();
		m_chartView->setChart(m_chart);
		m_chartView->chart()->legend()->hide();
		m_chartView->setContentsMargins(0, 0, 0, 0);

		//
		QHBoxLayout *mainLayout = new QHBoxLayout();

		this->setLayout(mainLayout);
	}

}//namespace Related
