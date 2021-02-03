/*!
 * @brief     自定义时间范围编辑控件   
 * @author    wey
 * @version   1.0
 * @date      2021.01.19 16:03:38
 * @warning
 * @copyright NanJing RenGu.
 * @note
 */
#pragma once

#include <QWidget>
#include <QDateTimeEdit>
#include <QLabel>
#include <QDate>

namespace Related {

	class TimeRangeEdit : public QWidget
	{
		Q_OBJECT

	public:
		TimeRangeEdit(QWidget *parent = nullptr);
		~TimeRangeEdit();

		void setText(QString tipInfo);

		void setTimeRange(QString startTime,  QString endTime);

		typedef QPair<QDateTime, QDateTime> TimeRange;
		TimeRange getTimeRange();

	private:
		void init();

	private:
		QLabel * m_tipInfoLabel;
		QDateTimeEdit * m_startTimeEdit;
		QDateTimeEdit * m_endTimeEdit;

	};

} //namespace Related 
