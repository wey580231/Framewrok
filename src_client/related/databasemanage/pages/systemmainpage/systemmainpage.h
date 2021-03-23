/*!
 *  @brief     ���ݸ�����ʾҳ��
 *  @details   ��ʾ��ǰ���ݿ��Ҫ��Ϣ��ͨ��ͼ������鼯��չʾ��
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

#include <base/util/rutil.h>
#include <base/selfwidget/iconbutton.h>
#include <base/selfwidget/rmessagebox.h>
#include <commondefines/protocol.h>

#include "overviewitem.h"
#include "../../customwidget/timerangeedit.h"
#include "../abstractpage.h"
#include "taskoverviewitem.h"
#include "newtaskdialog.h"
#include "filetransferdialog.h"

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
		 * @brief ��ָ������
		 * @param taskId ����ID
		 */
		void openTask(QString taskId);

		/*!
		 * @brief ɾ��ָ������
		 * @param taskId ����ID
		 */
		void deleteTask(QString taskId);

	private slots:
		void slotNewTaskClickde();
		void slotRefreshTaskClicked();
		void slotFileUploadListClicked();
		void slotSearchTaskClicked();
		void slotDeleteTask(QString taskId);
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
		Base::RIconButton * m_fileUploadButt;

		TimeRangeEdit * m_timeRange;
		QComboBox * m_locationBox;
		QComboBox * m_platBox;	

		QScrollArea * m_taskSrollArea;	
		QWidget * m_taskWindow;							/*!< ���񴰿��� */

		QList<TaskOverViewItem *> m_taskItems;			/*!< �����б� */	
		bool m_firstLoadData;							/*!< ��һ�μ���ҳ����ʾ */

		FileTransferDialog * m_fileUploadingDialog;		/*!< �ļ��ϴ����� */
	};

} //namespace Related