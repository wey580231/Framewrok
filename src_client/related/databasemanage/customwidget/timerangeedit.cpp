#include "timerangeedit.h"

#include <QLabel>
#include <QHboxLayout>

namespace Related {

	TimeRangeEdit::TimeRangeEdit(QWidget *parent)
		: QWidget(parent)
	{
		init();
	}

	TimeRangeEdit::~TimeRangeEdit()
	{
	}

	void TimeRangeEdit::setText(QString tipInfo)
	{
		m_tipInfoLabel->setText(tipInfo);
	}

	void TimeRangeEdit::setTimeRange(QString startTime, QString endTime)
	{
// 		QDate startTime = 
// 		m_startTimeEdit->setDate();
	}

	TimeRangeEdit::TimeRange TimeRangeEdit::getTimeRange()
	{
		return qMakePair(m_startTimeEdit->dateTime(), m_endTimeEdit->dateTime());
	}

	void TimeRangeEdit::init()
	{
		QHBoxLayout * timeLayout = new QHBoxLayout();

		m_tipInfoLabel = new QLabel();
		m_tipInfoLabel->setText(QStringLiteral("任务时间:"));

		QLabel * toTime = new QLabel();
		toTime->setText(QStringLiteral(" 至 "));

		m_startTimeEdit = new QDateTimeEdit();
		m_startTimeEdit->setDisplayFormat("yyyy-MM-dd");
		m_startTimeEdit->setMinimumHeight(25);

		m_endTimeEdit = new QDateTimeEdit();
		m_endTimeEdit->setDisplayFormat("yyyy-MM-dd");
		m_endTimeEdit->setDateTime(QDateTime::currentDateTime());
		m_endTimeEdit->setMinimumHeight(25);

		timeLayout->addWidget(m_tipInfoLabel);
		timeLayout->addWidget(m_startTimeEdit);
		timeLayout->addWidget(toTime);
		timeLayout->addWidget(m_endTimeEdit);

		setLayout(timeLayout);
	}

} //namespace Related 