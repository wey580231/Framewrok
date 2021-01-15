#include "SpectrogramWidget.h"

SpectrogramWidget::SpectrogramWidget(QWidget *parent)
	: QWidget(parent)
{
	//[] ’€œﬂÕº
	QLineSeries *lineSeries = new QLineSeries();
	for (int i = 0; i < 100; i++) {
		lineSeries->append(i, qrand() % 50);
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

	QChart::ChartTheme theme = QChart::ChartThemeBlueCerulean;
	if (theme == QChart::ChartThemeLight) {
		pal.setColor(QPalette::Window, QRgb(0xf0f0f0));
		pal.setColor(QPalette::WindowText, QRgb(0x404044));
	}
	else if (theme == QChart::ChartThemeDark) {
		pal.setColor(QPalette::Window, QRgb(0x121218));
		pal.setColor(QPalette::WindowText, QRgb(0xd6d6d6));
	}
	else if (theme == QChart::ChartThemeBlueCerulean) {
		pal.setColor(QPalette::Window, QRgb(0x40434a));
		pal.setColor(QPalette::WindowText, QRgb(0xd6d6d6));
	}
	else if (theme == QChart::ChartThemeBrownSand) {
		pal.setColor(QPalette::Window, QRgb(0x9e8965));
		pal.setColor(QPalette::WindowText, QRgb(0x404044));
	}
	else if (theme == QChart::ChartThemeBlueNcs) {
		pal.setColor(QPalette::Window, QRgb(0x018bba));
		pal.setColor(QPalette::WindowText, QRgb(0x404044));
	}
	else if (theme == QChart::ChartThemeHighContrast) {
		pal.setColor(QPalette::Window, QRgb(0xffab03));
		pal.setColor(QPalette::WindowText, QRgb(0x181818));
	}
	else if (theme == QChart::ChartThemeBlueIcy) {
		pal.setColor(QPalette::Window, QRgb(0xcee7f0));
		pal.setColor(QPalette::WindowText, QRgb(0x404044));
	}
	else {
		pal.setColor(QPalette::Window, QRgb(0xf0f0f0));
		pal.setColor(QPalette::WindowText, QRgb(0x404044));
	}
	window()->setPalette(pal);
	m_pChartView->chart()->setTheme(theme);
	bool checked = true;
	m_pChartView->chart()->setAnimationOptions(QChart::AllAnimations);

	QVBoxLayout *m_pQVBoxLayout = new QVBoxLayout();
	m_pQVBoxLayout->addWidget(m_pChartView);
	m_pQVBoxLayout->setContentsMargins(0,0,0,0);
	m_pQVBoxLayout->setSpacing(0);
	setLayout(m_pQVBoxLayout);

}

SpectrogramWidget::~SpectrogramWidget()
{
}

