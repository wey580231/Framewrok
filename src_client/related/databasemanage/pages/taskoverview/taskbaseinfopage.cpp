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
		// ����ȷֿ�ȱ�
		int  t_temp = t_i_wide / 4;
		// ���ƴ��ڷָ���
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
		//�������������Ϣ
		QWidget * leftWidget = new QWidget();
		{
			QLabel *t_taskname = new QLabel(QStringLiteral("�������ƣ�"));
			t_taskname->setAlignment(Qt::AlignCenter);
			m_taskname = new QLabel();
			m_taskname->setAlignment(Qt::AlignVCenter);

			QLabel *t_startTime = new QLabel(QStringLiteral("��ʼʱ�䣺"));
			t_startTime->setAlignment(Qt::AlignCenter);
			m_startTime = new QLabel();
			m_startTime->setAlignment(Qt::AlignVCenter);

			QLabel *t_endTime = new QLabel(QStringLiteral("����ʱ�䣺"));
			t_endTime->setAlignment(Qt::AlignCenter);
			m_endTime = new QLabel();
			m_endTime->setAlignment(Qt::AlignVCenter);

			QLabel *t_taskLon = new QLabel(QStringLiteral("���ȣ�"));
			t_taskLon->setAlignment(Qt::AlignCenter);
			m_taskLon = new QLabel();
			m_taskLon->setAlignment(Qt::AlignVCenter);

			QLabel *t_taskLat = new QLabel(QStringLiteral("γ�ȣ�"));
			t_taskLat->setAlignment(Qt::AlignCenter);
			m_taskLat = new QLabel();
			m_taskLat->setAlignment(Qt::AlignVCenter);


			QGridLayout *leftLayout = new QGridLayout();
			leftLayout->addWidget(t_taskname, 0, 0);
			leftLayout->addWidget(m_taskname, 0, 1);

			leftLayout->addWidget(t_startTime, 1, 0);
			leftLayout->addWidget(m_startTime, 1, 1);

			leftLayout->addWidget(t_endTime, 2, 0);
			leftLayout->addWidget(m_endTime, 2, 1);

			leftLayout->addWidget(t_taskLon, 3, 0);
			leftLayout->addWidget(m_taskLon, 3, 1);

			leftLayout->addWidget(t_taskLat, 4, 0);
			leftLayout->addWidget(m_taskLat, 4, 1);
			leftWidget->setLayout(leftLayout);
		}

		QWidget * middle1Widget = new QWidget();
		{
			QLabel *t_taskLocation = new QLabel(QStringLiteral("����ص㣺"));
			t_taskLocation->setAlignment(Qt::AlignCenter);
			m_taskLocation = new QLabel();
			m_taskLocation->setAlignment(Qt::AlignVCenter);

			QLabel *t_testImages = new QLabel(QStringLiteral("����ͼƬͳ�ƣ�"));
			t_testImages->setAlignment(Qt::AlignCenter);
			QLabel  *m_testImages = new QLabel();
			m_testImages->setText(QString::number(15));
			m_taskname->setAlignment(Qt::AlignVCenter);

			QLabel *t_dutyRecord = new QLabel(QStringLiteral("ֵ����־ͳ�ƣ�"));
			t_dutyRecord->setAlignment(Qt::AlignCenter);
			QLabel *m_dutyRecord = new QLabel();
			m_dutyRecord->setText(QString::number(10));
			m_dutyRecord->setAlignment(Qt::AlignVCenter);

			QLabel *t_experimentRecord = new QLabel(QStringLiteral("�����¼ͳ�ƣ�"));
			t_experimentRecord->setAlignment(Qt::AlignCenter);
			QLabel * m_experimentRecord = new QLabel();
			m_experimentRecord->setText(QString::number(10));
			m_experimentRecord->setAlignment(Qt::AlignVCenter);

			QLabel * t_testResults = new QLabel(QStringLiteral("������ͳ�ƣ�"));
			t_testResults->setAlignment(Qt::AlignCenter);
			QLabel * m_testResults = new QLabel();
			m_testResults->setText(QString::number(10));
			m_testResults->setAlignment(Qt::AlignVCenter);

			QGridLayout *middle1Layout = new QGridLayout();
			middle1Layout->addWidget(t_taskLocation, 0, 0);
			middle1Layout->addWidget(m_taskLocation, 0, 1);
			middle1Layout->addWidget(t_testImages, 1, 0);
			middle1Layout->addWidget(m_testImages, 1, 1);
			middle1Layout->addWidget(t_dutyRecord, 2, 0);
			middle1Layout->addWidget(m_dutyRecord, 2, 1);
			middle1Layout->addWidget(t_experimentRecord, 3, 0);
			middle1Layout->addWidget(m_experimentRecord, 3, 1);
			middle1Layout->addWidget(t_testResults, 4, 0);
			middle1Layout->addWidget(m_testResults, 4, 1);

			middle1Layout->setContentsMargins(0, 0, 0, 0);
			middle1Widget->setLayout(middle1Layout);
		}

		// ƽ̨���ͱ���
		QWidget * middleWidget = new QWidget();
		{
			QLabel *t_platformtype = new QLabel(QStringLiteral("ƽ̨���ͣ�"));
			t_platformtype->setAlignment(Qt::AlignCenter);
			m_platformtype = new QLabel();
			m_platformtype->setAlignment(Qt::AlignVCenter);

			QLabel *t_platformnunber = new QLabel(QStringLiteral("ƽ̨������"));
			t_platformnunber->setAlignment(Qt::AlignCenter);
			m_platformnunber = new QLabel();
			m_platformnunber->setAlignment(Qt::AlignVCenter);

			QLabel *t_platforSze = new QLabel(QStringLiteral("ƽ̨���ݴ�С��"));
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

		//��ʾƽ̨���ݱ���
		QWidget * rightWidget = new QWidget();
		{
			QLabel *t_advNumber = new QLabel(QStringLiteral("��Ƶͳ��ֵ��"));
			t_advNumber->setAlignment(Qt::AlignCenter);
			m_advNumber = new QLabel();
			m_advNumber->setAlignment(Qt::AlignVCenter);

			QLabel *t_advSize = new QLabel(QStringLiteral("��Ƶ���ݴ�С��"));
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

		//[] �����沼��
		QHBoxLayout *mainLayout = new QHBoxLayout();
		mainLayout->addWidget(leftWidget,1);
		mainLayout->addWidget(middle1Widget, 1);
		mainLayout->addWidget(middleWidget,1);
		mainLayout->addWidget(rightWidget,1);
		mainLayout->setContentsMargins(4, 4, 4, 4);
		this->setLayout(mainLayout);
	}

}