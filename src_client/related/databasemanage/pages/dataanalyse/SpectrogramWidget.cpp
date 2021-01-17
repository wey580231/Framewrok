#include "SpectrogramWidget.h"

SpectrogramWidget::SpectrogramWidget(QWidget *parent)
	: QWidget(parent)
{
	//[] ’€œﬂÕº
	QLineSeries *lineSeries = new QLineSeries();
	for (int i = 0; i < 100; i++) {
		lineSeries->append(i, qrand() % 10);
	}

	QChart * m_pChart = new QChart();
	m_pChart->setTitle("dome");
	m_pChart->addSeries(lineSeries);
	m_pChart->createDefaultAxes();
	m_pChart->setMargins(QMargins(0, 0, 0, 0));
	m_pChart->setContentsMargins(0, 0, 0, 0);
	m_pChart->setTheme(QChart::ChartThemeDark);

	QChartView *m_pChartView = new QChartView(m_pChart);
	m_pChartView->setObjectName("widgetContainer");
	m_pChartView->setRenderHint(QPainter::Antialiasing);
	m_pChartView->setContentsMargins(0, 0, 0, 0);
	m_pChartView->chart()->legend()->hide();
	
	QPalette pal1 = qApp->palette();
	pal1.setColor(QPalette::Window, QRgb(0xf0f0f0));
	pal1.setColor(QPalette::WindowText, QRgb(0x404044));
	qApp->setPalette(pal1);

	QPalette pal = window()->palette();

	m_pChartView->chart()->setBackgroundBrush(QColor(0,0,0,0));
	m_pChartView->chart()->setAnimationOptions(QChart::NoAnimation);

	QVBoxLayout *m_pQVBoxLayout = new QVBoxLayout();
	m_pQVBoxLayout->addWidget(m_pChartView);
	m_pQVBoxLayout->setContentsMargins(0,0,0,0);
	m_pQVBoxLayout->setSpacing(0);
	setLayout(m_pQVBoxLayout);

}

SpectrogramWidget::~SpectrogramWidget()
{
}

