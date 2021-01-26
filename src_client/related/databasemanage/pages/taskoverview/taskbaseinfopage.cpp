#include "taskbaseinfopage.h"

namespace Related {

	TaskBaseInfoPage::TaskBaseInfoPage(QWidget *parent)
		: QWidget(parent)
	{
		init();
	}

	TaskBaseInfoPage::~TaskBaseInfoPage()
	{
	}

	void TaskBaseInfoPage::setTaskBaseInfo(TaskBaseInfo info)
	{
		m_taskBaseInfo = info;
		//刷新任务信息
		m_taskname->setText(m_taskBaseInfo.taskName);
		//m_taskTime->setText(m_taskBaseInfo.taskTime);
		m_taskplace->setText(m_taskBaseInfo.taskplace);
		m_startTime->setText(m_taskBaseInfo.startTime);
		m_endTime->setText(m_taskBaseInfo.endTime);
		m_timeLength->setText(m_taskBaseInfo.timeLength);

	}

	void TaskBaseInfoPage::paintEvent(QPaintEvent * event)
	{
		int t_i_wide = this->width();
		int t_i_high = this->height();
		// 计算等分宽度比
		int  t_temp = t_i_wide / 4;
		// 绘制窗口分割线
		QPainter painter(this);
		painter.setRenderHint(QPainter::Antialiasing);
		painter.setRenderHint(QPainter::SmoothPixmapTransform);
		painter.save();
		for (int i = 1; i < 4; i++) {
			QLineF line(t_temp * i, 0 + 20, t_temp * i, t_i_high - 20);
			painter.drawLine(line);
		}
		painter.restore();
		QWidget::paintEvent(event);
	}

	void TaskBaseInfoPage::init()
	{
		//任务基本描述信息
		QWidget * leftWidget = new QWidget();
		{
			QLabel *t_taskname = new QLabel(QStringLiteral("任务名称："));
			t_taskname->setAlignment(Qt::AlignCenter);
			m_taskname = new QLabel();
			m_taskname->setAlignment(Qt::AlignVCenter);

			QLabel *t_taskTime = new QLabel(QStringLiteral("任务时间："));
			t_taskTime->setAlignment(Qt::AlignCenter);
			m_taskTime = new QLabel();
;			m_taskTime->setAlignment(Qt::AlignVCenter);

			QLabel *t_taskplace = new QLabel(QStringLiteral("试验地点："));
			t_taskplace->setAlignment(Qt::AlignCenter);
			m_taskplace = new QLabel();
			m_taskplace->setAlignment(Qt::AlignVCenter);

			QLabel *t_pTestUser = new QLabel(QStringLiteral("试验人员："));
			t_pTestUser->setAlignment(Qt::AlignCenter);
			QLabel *m_TestUser = new QLabel();
			m_TestUser->setAlignment(Qt::AlignVCenter);

			QGridLayout *leftLayout = new QGridLayout();
			leftLayout->addWidget(t_taskname, 0, 0);
			leftLayout->addWidget(m_taskname, 0, 1);
			leftLayout->addWidget(t_taskTime, 1, 0);
			leftLayout->addWidget(m_taskTime, 1, 1);
			leftLayout->addWidget(t_taskplace, 2, 0);
			leftLayout->addWidget(m_taskplace, 2, 1);
			leftLayout->addWidget(t_pTestUser, 3, 0);
			leftLayout->addWidget(m_TestUser, 3, 1);
			leftWidget->setLayout(leftLayout);
		}

		QWidget * middle1Widget = new QWidget();
		{
			QLabel *t_startTime = new QLabel(QStringLiteral("起始时间："));
			t_startTime->setAlignment(Qt::AlignCenter);
			m_startTime = new QLabel();
			m_startTime->setAlignment(Qt::AlignVCenter);

			QLabel *t_endTime = new QLabel(QStringLiteral("结束时间："));
			t_endTime->setAlignment(Qt::AlignCenter);
			m_endTime = new QLabel();
			m_endTime->setAlignment(Qt::AlignVCenter);

			QLabel *t_timeLength = new QLabel(QStringLiteral("时长："));
			t_timeLength->setAlignment(Qt::AlignCenter);
			m_timeLength = new QLabel();
			m_timeLength->setAlignment(Qt::AlignVCenter);

			QLabel * t_dataLength = new QLabel(QStringLiteral("试验数据大小："));
			t_dataLength->setAlignment(Qt::AlignCenter);

			QLabel * t_datafilesize = new QLabel();
			t_datafilesize->setAlignment(Qt::AlignVCenter);

			QGridLayout *middle1Layout = new QGridLayout();
			middle1Layout->addWidget(t_startTime, 0, 0);
			middle1Layout->addWidget(m_startTime, 0, 1);
			middle1Layout->addWidget(t_endTime, 1, 0);
			middle1Layout->addWidget(m_endTime, 1, 1);
			middle1Layout->addWidget(t_timeLength, 2, 0);
			middle1Layout->addWidget(m_timeLength, 2, 1);
			middle1Layout->addWidget(t_dataLength, 3, 0);
			middle1Layout->addWidget(t_datafilesize, 3, 1);
			middle1Layout->setContentsMargins(0, 0, 0, 0);
			middle1Widget->setLayout(middle1Layout);
		}

		// 平台类型比例
		QWidget * middleWidget = new QWidget();
		{
			QLabel *t_platformtype = new QLabel(QStringLiteral("平台类型："));
			t_platformtype->setAlignment(Qt::AlignCenter);
			m_platformtype = new QLabel();
			m_platformtype->setAlignment(Qt::AlignVCenter);

			QLabel *t_platformnunber = new QLabel(QStringLiteral("平台个数："));
			t_platformnunber->setAlignment(Qt::AlignCenter);
			m_platformnunber = new QLabel();
			m_platformnunber->setAlignment(Qt::AlignVCenter);

			QLabel *t_platforSze = new QLabel(QStringLiteral("平台数据大小："));
			t_platforSze->setAlignment(Qt::AlignCenter);
			m_platforSze = new QLabel();
			m_platforSze->setAlignment(Qt::AlignVCenter);
	
			QLabel *t_pTenp2 = new QLabel(QStringLiteral("平台编号："));
			t_pTenp2->setAlignment(Qt::AlignCenter);
			QLabel *m_pTenp3= new QLabel();
			m_pTenp3->setAlignment(Qt::AlignVCenter);

			QGridLayout *middleLayout = new QGridLayout();
			middleLayout->addWidget(t_platformtype, 0, 0);
			middleLayout->addWidget(m_platformtype, 0, 1);
			middleLayout->addWidget(t_platformnunber, 1, 0);
			middleLayout->addWidget(m_platformnunber, 1, 1);
			middleLayout->addWidget(t_platforSze, 2, 0);
			middleLayout->addWidget(m_platforSze, 2, 1);
			middleLayout->addWidget(t_pTenp2, 3, 0);
			middleLayout->addWidget(m_pTenp3, 3, 1);

			middleLayout->setContentsMargins(0, 0, 0, 0);

			middleWidget->setLayout(middleLayout);
		}

		//显示平台数据比例
		QWidget * rightWidget = new QWidget();
		{
			QLabel *t_advNumber = new QLabel(QStringLiteral("音频统计值："));
			t_advNumber->setAlignment(Qt::AlignCenter);
			m_advNumber = new QLabel();
			m_advNumber->setAlignment(Qt::AlignVCenter);

			QLabel *t_advSize = new QLabel(QStringLiteral("音频数据大小："));
			t_advSize->setAlignment(Qt::AlignCenter);
			m_advSize = new QLabel();
			m_advSize->setAlignment(Qt::AlignVCenter);

			QLabel *t_temp = new QLabel();
			QLabel * t_temp2 = new QLabel();

			QGridLayout *rightLayout = new QGridLayout();
			rightLayout->addWidget(t_advNumber, 0, 0);
			rightLayout->addWidget(m_advNumber, 0, 1);
			rightLayout->addWidget(t_advSize, 1, 0);
			rightLayout->addWidget(m_advSize, 1, 1);
			rightLayout->addWidget(t_temp, 2, 0);
			rightLayout->addWidget(t_temp2, 2, 1);
			rightLayout->setContentsMargins(0, 0, 0, 0);
			
			rightWidget->setLayout(rightLayout);
		}

		//[] 主界面布局
		QHBoxLayout *mainLayout = new QHBoxLayout();
		mainLayout->addWidget(leftWidget,1);
		mainLayout->addWidget(middle1Widget, 1);
		mainLayout->addWidget(middleWidget,1);
		mainLayout->addWidget(rightWidget,1);
		mainLayout->setContentsMargins(4, 4, 4, 4);
		this->setLayout(mainLayout);
	}

	void TaskBaseInfoPage::createPlatformTypeCartogram()
	{
		QPieSlice *slice_1 = new QPieSlice(QStringLiteral("平台1数据"), 0.6);
		//slice_1->setLabelVisible(true); 
		slice_1->setBrush(Qt::green);
		QPieSlice *slice_2 = new QPieSlice(QStringLiteral("平台1数据"), 0.4);
		//slice_2->setLabelVisible(true);
		slice_2->setBrush(Qt::blue);

		// 将两个饼状分区加入series
		QPieSeries *series = new QPieSeries();
		series->append(slice_1);
		series->append(slice_2);

		QChart *chart = new QChart();
		chart->addSeries(series);
		//chart->setAnimationOptions(QChart::AllAnimations); 

		chart->setTheme(QChart::ChartThemeBlueCerulean);

		chart->setMargins(QMargins(0, 0, 0, 0));
		chart->setContentsMargins(0, 0, 0, 0);
		chart->layout()->setContentsMargins(0, 0, 0, 0);//设置外边界全部为0
		chart->setBackgroundRoundness(0);//设置背景区域无圆角

		platformTypeView = new QChartView();
		platformTypeView->setChart(chart);
		platformTypeView->chart()->legend()->hide();
		platformTypeView->setContentsMargins(0, 0, 0, 0);
		platformTypeView->setRenderHint(QPainter::Antialiasing);
	}

	void TaskBaseInfoPage::platformDataCartogram()
	{
		QPieSlice *slice_1 = new QPieSlice(QStringLiteral("平台1数据"), 0.6);
		//slice_1->setLabelVisible(true); 
		slice_1->setBrush(Qt::green);
		QPieSlice *slice_2 = new QPieSlice(QStringLiteral("平台1数据"), 0.4);
		//slice_2->setLabelVisible(true);
		slice_2->setBrush(Qt::blue);

		// 将两个饼状分区加入series
		QPieSeries *series = new QPieSeries();
		series->append(slice_1);
		series->append(slice_2);

		QChart *chart = new QChart();
		chart->addSeries(series);
		//chart->setAnimationOptions(QChart::AllAnimations); 

		chart->setTheme(QChart::ChartThemeBlueCerulean);
		//chart->setBackgroundVisible(false);  //去背景

// 		QStyle *style = QStyleFactory::create(QString("background-color:rgba(0,77,136, 255);"));
// 
// 		chart->setStyle(style);
		chart->setMargins(QMargins(0, 0, 0, 0));
		chart->setContentsMargins(0, 0, 0, 0);
		chart->layout()->setContentsMargins(0, 0, 0, 0);
		chart->setBackgroundRoundness(0);

		chartview = new QChartView();
		chartview->setChart(chart);
		chartview->chart()->legend()->hide();
		chartview->setContentsMargins(0, 0, 0, 0);
		chartview->setRenderHint(QPainter::Antialiasing);

	}

}