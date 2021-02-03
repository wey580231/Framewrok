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
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QDateTime>
#include <QListView>
#include <QMap>

#include <base/selfwidget/iconbutton.h>
#include <base/selfwidget/rmessagebox.h>
#include <commondefines/protocol.h>

#include "overviewitem.h"
#include "../../customwidget/timerangeedit.h"
#include "../abstractpage.h"
#include "taskoverviewitem.h"
#include "newtaskdialog.h"

namespace Related {

	class TaskOverViewItem;

	class SystemMainPage : public AbstractPage
	{
		Q_OBJECT

	public:
		SystemMainPage(QWidget *parent = nullptr);
		~SystemMainPage();

		PageType getPageType() const;
		void prepareBringToTop();

	signals:
		/*!
		 * @brief 打开指定任务
		 * @param taskId 任务ID
		 */
		void openTask(QString taskId);

		/*!
		 * @brief 删除指定任务
		 * @param taskId 任务ID
		 */
		void deleteTask(QString taskId);

	private slots:
		void slotNewTaskClickde();
		void slotRefreshTaskClicked();
		void slotDeleteTask(QString taskId);
		void slotLocationActivated(QString index);
		void slotPlatActivated(QString index);

		void processQueryAllTaskResponse(const Datastruct::LoadAllTaskResponse & response);
		void processTaskByConditionResponse(const Datastruct::TaskByConditionResponse & response);
		void processTaskDeleteResponse(const Datastruct::TaskDeleteResponse & response);
		void processTaskStaticsInfoResponse(const Datastruct::TaskStaticsInfoResponse & response);

	private:
		void init();
		void initConnent();

		void refreshCurrTask();
		void refreshTaskByCondition(Datastruct::TaskByConditionRequest request);
		void updateTaskListWidget();

	private:
		OverViewItem * m_taskNumItem;
		OverViewItem * m_diskSpaceItem;
		OverViewItem * m_platNumItem;

		Base::RIconButton * m_newTaskButt;				
		Base::RIconButton * m_refreshTaskButt;
		Base::RIconButton * m_searchTaskButt;

		TimeRangeEdit * m_timeRange;
		QComboBox * m_locationBox;
		QComboBox * m_platBox;	

		QScrollArea * m_taskSrollArea;	
		QWidget * m_taskWindow;							/*!< 任务窗口区 */

		QList<TaskOverViewItem *> m_taskItems;			/*!< 任务列表 */	
		bool m_firstLoadData;	/*!< 第一次加载页面显示 */
	};

} //namespace Related