/*!
 *  @brief     数据概览显示页面
 *  @details   显示当前数据库概要信息，通过图标或区块集中展示。
 *  @author    wey
 *  @version   1.0
 *  @date      2021.01.07
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#pragma once

#include <QWidget>
#include <QDateTimeEdit>
#include <QComboBox>
#include <QtWidgets/QScrollArea>

#include <base/selfwidget/iconbutton.h>

#include "overviewitem.h"
#include "../../customwidget/timerangeedit.h"
#include "../abstractpage.h"
#include "../../3rdLibrary/qcustomplot.h"

namespace Related {

	class TaskOverViewItem;

	class MainPage : public AbstractPage
	{
		Q_OBJECT

	public:
		MainPage(QWidget *parent = nullptr);
		~MainPage();

		PageType getPageType() const;

	private slots:
		/*!
		 * @brief 打开指定任务
		 * @param taskId 任务ID
		 */
		void respOpenTask(QString taskId);

		/*!
		 * @brief 删除指定任务
		 * @param taskId 任务ID
		 */
		void respDeleteTask(QString taskId);

	private:
		void init();
		void initTaskList();

	private:
		OverViewItem * m_taskNumItem;
		OverViewItem * m_diskSpaceItem;
		OverViewItem * m_platNumItem;

		Base::RIconButton * m_newTaskButt;
		Base::RIconButton * m_refreshTaskButt;

		TimeRangeEdit * m_timeRange;
		QComboBox * m_locationBox;
		QComboBox * m_platBox;	

		QScrollArea * m_taskSrollArea;	
		QWidget * m_taskWindow;			/*!< 任务窗口区 */

		QList<TaskOverViewItem *> m_taskItems;
	};

} //namespace Related