/*!
 * @brief     任务基本信息窗口
 * @details   描述任务的基本参数信息 如：时间、地点等
 * @author    yzg
 * @version   1.0
 * @date      2021.01.20 16:08:58
 * @warning
 * @copyright NanJing RenGu.
 * @note
 */
#pragma once

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QPainter>

 //[] 
#include <QtCharts>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QChartView>

#include "../../datastruct.h"

#include "statisticalgraph/statisticalgraphpage.h"

namespace Related {

	class TaskBaseInfoPage : public QWidget
	{
		Q_OBJECT

	public:
		TaskBaseInfoPage(QWidget *parent = nullptr);
		~TaskBaseInfoPage();

		void setTaskBaseInfo(TaskBaseInfo info);


	protected:
		void paintEvent(QPaintEvent *event);

	private:
		void init();
		void createPlatformTypeCartogram();

		void platformDataCartogram();

	private:
		QLabel *m_taskname;
		QLabel *m_taskTime;
		QLabel *m_taskplace;

		QLabel *m_startTime;
		QLabel *m_endTime;
		QLabel *m_timeLength;
		QLabel *m_datafilesize;


		QLabel *m_platformtype;
		QLabel *m_platformnunber;
		QLabel *m_platforSze;

		QLabel *m_advNumber;
		QLabel *m_advSize;


		TaskBaseInfo m_taskBaseInfo;



		QChartView *platformTypeView;
		QChartView *platformDataView;

		QChartView *chartview;

		StatisticalGraphPage * m_platformTypeItem;				/*!< 平台类型统计 */
		StatisticalGraphPage * m_platformDataItem;				/*!< 平台数据统计信息 */

	};

}//namespace Related 