/*!
 * @brief    任务数据结果系信息展示     
 * @author    yzg
 * @version   1.0
 * @date      2021.01.21 10:19:41
 * @warning
 * @copyright NanJing RenGu.
 * @note
 */
 
#pragma once

#include <QWidget>

#include <QScrollArea>
#include <QScrollBar>
#include <QLabel>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>

#include "../../datastruct.h"
#include "taskresultitem.h"
#include <base\selfwidget\iconbutton.h>
#include <base\selfwidget\rmessagebox.h>

namespace Related {

	class TaskResultInfoPage : public QWidget
	{
		Q_OBJECT

	public:
		TaskResultInfoPage(QWidget *parent = nullptr);
		~TaskResultInfoPage();

		void  setResultInfos(PlatformInfos info);

	protected slots: 
		void slotSeletePictureIndex(int index, QString path);
		void slotLeftSwitchWidget();
		void slotrightSwitchWidget();

	private:
		void init();
		void createTaskResult();
	private:
		TaskResultItem * m_previewWidget;
		Base::RIconButton * m_leftButton;
		Base::RIconButton * m_rightButton;

		QScrollArea *m_resultScrollArea;
		QWidget *m_resultWidget;
		QList<TaskResultItem *> m_resultInfos;

		int m_curveIndex;							/*!< 当前图片索引 */
	};

}//namespace Related 