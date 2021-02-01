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

#include <base/util/rutil.h>
#include <commondefines/structdefines.h>

#include "../../datastruct.h"

namespace Related {

	class TaskBaseInfoPage : public QWidget
	{
		Q_OBJECT

	public:
		TaskBaseInfoPage(QWidget *parent = nullptr);
		~TaskBaseInfoPage();

		void setTaskBaseInfo(Datastruct::TaskEntityData info);


	protected:
		void paintEvent(QPaintEvent *event);

	private:
		void init();

	private:
		QLabel * m_taskname;
		QLabel * m_startTime;
		QLabel * m_endTime;

		QLabel * m_taskLocation;
		QLabel * m_taskLon;
		QLabel * m_taskLat;

		QLabel * m_description;

		QLabel *m_platformtype;
		QLabel *m_platformnunber;
		QLabel *m_platforSze;

		QLabel *m_advNumber;
		QLabel *m_advSize;


		TaskBaseInfo m_taskBaseInfo;


	};

}//namespace Related 