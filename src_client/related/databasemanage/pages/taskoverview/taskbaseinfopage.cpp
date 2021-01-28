#include "taskbaseinfopage.h"

#include <QDebug>

namespace Related {

	TaskBaseInfoPage::TaskBaseInfoPage(QWidget *parent)
		: QWidget(parent)
	{
		init();
	}

	TaskBaseInfoPage::~TaskBaseInfoPage()
	{
	}

	void TaskBaseInfoPage::setTaskBaseInfo(Datastruct::TaskEntityData info)
	{
		m_taskname->setText(info.taskName);
		m_startTime->setText(info.startTime);
		m_endTime->setText(info.endTime);

		m_taskLocation->setText(info.location);
		m_taskLon->setText(info.lon);
		m_taskLat->setText(info.lat);

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

			QLabel *t_startTime = new QLabel(QStringLiteral("起始时间："));
			t_startTime->setAlignment(Qt::AlignCenter);
			m_startTime = new QLabel();
			m_startTime->setAlignment(Qt::AlignVCenter);

			QLabel *t_endTime = new QLabel(QStringLiteral("结束时间："));
			t_endTime->setAlignment(Qt::AlignCenter);
			m_endTime = new QLabel();
			m_endTime->setAlignment(Qt::AlignVCenter);

			QGridLayout *leftLayout = new QGridLayout();
			leftLayout->addWidget(t_taskname, 0, 0);
			leftLayout->addWidget(m_taskname, 0, 1);
			leftLayout->addWidget(t_startTime, 1, 0);
			leftLayout->addWidget(m_startTime, 1, 1);
			leftLayout->addWidget(t_endTime, 2, 0);
			leftLayout->addWidget(m_endTime, 2, 1);
			leftWidget->setLayout(leftLayout);
		}

		QWidget * middle1Widget = new QWidget();
		{
			QLabel *t_taskLocation = new QLabel(QStringLiteral("试验地点："));
			t_taskLocation->setAlignment(Qt::AlignCenter);
			m_taskLocation = new QLabel();
			m_taskLocation->setAlignment(Qt::AlignVCenter);

			QLabel *t_taskLon = new QLabel(QStringLiteral("经度："));
			t_taskLon->setAlignment(Qt::AlignCenter);
			m_taskLon = new QLabel();
			m_taskLon->setAlignment(Qt::AlignVCenter);

			QLabel *t_taskLat = new QLabel(QStringLiteral("纬度："));
			t_taskLat->setAlignment(Qt::AlignCenter);
			m_taskLat = new QLabel();
			m_taskLat->setAlignment(Qt::AlignVCenter);

			QGridLayout *middle1Layout = new QGridLayout();
			middle1Layout->addWidget(t_taskLocation, 0, 0);
			middle1Layout->addWidget(m_taskLocation, 0, 1);
			middle1Layout->addWidget(t_taskLon, 1, 0);
			middle1Layout->addWidget(m_taskLon, 1, 1);
			middle1Layout->addWidget(t_taskLat, 2, 0);
			middle1Layout->addWidget(m_taskLat, 2, 1);

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
	

			QGridLayout *middleLayout = new QGridLayout();
			middleLayout->addWidget(t_platformtype, 0, 0);
			middleLayout->addWidget(m_platformtype, 0, 1);
			middleLayout->addWidget(t_platformnunber, 1, 0);
			middleLayout->addWidget(m_platformnunber, 1, 1);
			middleLayout->addWidget(t_platforSze, 2, 0);
			middleLayout->addWidget(m_platforSze, 2, 1);


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

}