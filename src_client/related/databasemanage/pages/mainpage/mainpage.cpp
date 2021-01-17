#include "mainpage.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QDateTime>

#include "../../customwidget/customwidgetcontainer.h"


namespace Related {

	MainPage::MainPage(QWidget *parent)
		: AbstractPage(parent)
	{
		init();
	}

	MainPage::~MainPage()
	{
	}

	Related::PageType MainPage::getPageType() const
	{
		return Page_MainPage;
	}

	void MainPage::init()
	{
		QWidget * mainWidget = new QWidget();

		CustomWidgetContainer * overViewContainer = new CustomWidgetContainer();

		QSize maxSize(415, 170);
		QSize minSize(345, 170);
		m_todayAddItem = new OverViewItem();
		m_todayAddItem->setLabelText(QStringLiteral("今日新增条数"));
		m_todayAddItem->setLabelBackground(QColor(237, 168, 27));
		m_todayAddItem->setLabelIcon(QStringLiteral(":/QYBlue/resource/qyblue/上传.png"));
		m_todayAddItem->setMaximumSize(maxSize);
		m_todayAddItem->setMinimumSize(minSize);

		m_totalItem = new OverViewItem();
		m_totalItem->setLabelBackground(QColor(77, 174, 116));
		m_totalItem->setLabelText(QStringLiteral("识别库条数"));
		m_totalItem->setLabelIcon(QStringLiteral(":/QYBlue/resource/qyblue/数据库记录.png"));
		m_totalItem->setMaximumSize(maxSize);
		m_totalItem->setMinimumSize(minSize);

		m_unprocessItem = new OverViewItem();
		m_unprocessItem->setLabelBackground(QColor(199, 99, 116));
		m_unprocessItem->setLabelText(QStringLiteral("未处理条数"));
		m_unprocessItem->setLabelIcon(QStringLiteral(":/QYBlue/resource/qyblue/未处理.png"));
		m_unprocessItem->setMaximumSize(maxSize);
		m_unprocessItem->setMinimumSize(minSize);

		QHBoxLayout * overViewLayout = new QHBoxLayout();
		overViewLayout->setContentsMargins(10, 15, 10, 15);
		overViewLayout->addWidget(m_todayAddItem);
		overViewLayout->addWidget(m_totalItem);
		overViewLayout->addWidget(m_unprocessItem);

		QWidget * tmpWidget = new QWidget();
		tmpWidget->setLayout(overViewLayout);
		overViewContainer->setContent(tmpWidget);

		CustomWidgetContainer * leftChart = new CustomWidgetContainer();
		leftChart->setMinimumHeight(450);

		CustomWidgetContainer * rightChart = new CustomWidgetContainer();
		rightChart->setMinimumHeight(450);

		//左侧一周数据统计
		{
			QCustomPlot * leftPlot = new QCustomPlot();
			leftChart->setContent(leftPlot);
			initPlot(leftPlot);

			QCPAxis *keyAxis = leftPlot->xAxis;
			QCPAxis *valueAxis = leftPlot->yAxis;
			QCPBars * bars = new QCPBars(keyAxis, valueAxis);

			bars->setAntialiased(false);
			bars->setName("bars fuels");
			bars->setPen(QPen(QColor(0, 168, 140).lighter(130)));
			bars->setBrush(QColor(0, 168, 140));

			QVector<double> ticks;
			QVector<QString> labels;

			for (int i = 0; i < 7; i++)
			{
				ticks << i + 1;

				QDateTime cdateTime = QDateTime::currentDateTime();
				cdateTime = cdateTime.addDays(i - 6);
				labels << cdateTime.toString("MM-dd");
			}

			QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
			textTicker->addTicks(ticks, labels);
			keyAxis->setTicker(textTicker);

			keyAxis->setSubTicks(false);
			keyAxis->setTickLength(0, 4);
			keyAxis->setRange(0, 8);
			keyAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);

			valueAxis->setPadding(35);
			valueAxis->setLabel(QStringLiteral("最近一周统计"));
			valueAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);

			QVector<double> barsData;
			barsData << 35 << 18 << 24 << 41 << 10 << 8 << 39;
			bars->setData(ticks, barsData);

			valueAxis->rescale(true);
			leftPlot->replot();
		}

		//右侧三库数据统计
		{
			QCustomPlot * rightPlot = new QCustomPlot();
			initPlot(rightPlot);
			rightChart->setContent(rightPlot);

			QCPAxis *keyAxis = rightPlot->xAxis;
			QCPAxis *valueAxis = rightPlot->yAxis;
			QCPBars * bars = new QCPBars(keyAxis, valueAxis);

			bars->setAntialiased(false);
			bars->setName("bars fuels");
			bars->setPen(QPen(QColor(0, 168, 140).lighter(130)));
			bars->setBrush(QColor(0, 168, 140));

			QVector<double> ticks;
			QVector<QString> labels;

			ticks << 1 << 2 << 3;
			labels << QStringLiteral("0级库") << QStringLiteral("1级库") << QStringLiteral("2级库");

			QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
			textTicker->addTicks(ticks, labels);
			keyAxis->setTicker(textTicker);

			keyAxis->setSubTicks(false);
			keyAxis->setTickLength(0, 4);
			keyAxis->setRange(0, 4);
			keyAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);

			valueAxis->setPadding(35);
			valueAxis->setLabel(QStringLiteral("三库数据统计"));
			valueAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);

			QVector<double> barsData;
			barsData << 167 << 210 << 349;
			bars->setData(ticks, barsData);

			valueAxis->rescale(true);
			rightPlot->replot();
		}

		QHBoxLayout * chartLayout = new QHBoxLayout();
		chartLayout->setContentsMargins(0, 0, 0, 0);
		chartLayout->addWidget(leftChart);
		chartLayout->addWidget(rightChart);
		chartLayout->setStretch(0, 1);
		chartLayout->setStretch(1, 1);

		QVBoxLayout * mainLayout = new QVBoxLayout();
		mainLayout->setContentsMargins(0, 0, 0, 0);
		mainLayout->addWidget(overViewContainer);
		mainLayout->addLayout(chartLayout);
		mainLayout->addStretch(1);

		mainWidget->setLayout(mainLayout);

		QHBoxLayout * layout = new QHBoxLayout();
		layout->setContentsMargins(4, 4, 4, 4);
		layout->addWidget(mainWidget);
		setLayout(layout);
	}

	void MainPage::initPlot(QCustomPlot * plot)
	{
		plot->setBackground(QColor(0, 0, 0, 0));

		auto setAxisStyle = [](QCPAxis * axis) {
			QColor baseColor(118, 121, 124);
			axis->setBasePen(QPen(baseColor, 1.5));
			axis->setTickPen(QPen(baseColor, 1.5));
			axis->setSubTickPen(baseColor);

			QColor tickColor(143, 222, 231);
			QFont labelFont(QStringLiteral("微软雅黑"), 11);
			axis->setTickLabelFont(labelFont);
			axis->setTickLabelColor(tickColor);

			labelFont.setPixelSize(13);
			axis->setLabelFont(labelFont);
			axis->setLabelColor(tickColor);
		};

		setAxisStyle(plot->xAxis);
		setAxisStyle(plot->yAxis);
	}

} //namespace Related 