/*!
 * @brief    任务图片信息展示窗口   
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

#include <base\selfwidget\iconbutton.h>
#include <base\selfwidget\rmessagebox.h>

#include "../../datastruct.h"

#include "taskresultitem.h"


namespace Related {

	class TaskResultInfoPage : public QWidget
	{
		Q_OBJECT

	public:
		TaskResultInfoPage(QWidget *parent = nullptr);
		~TaskResultInfoPage();

	protected slots: 

	private:
		void init();
		void updateTestImages();
		void createImagesItem();

	private:
		QScrollArea *m_resultScrollArea;
		QWidget *m_imagesWidget;
		QList<TestImagesItem *> m_imagesItems;
	};

}//namespace Related 