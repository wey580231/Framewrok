/*!
 * @brief     任务报告编辑与展示界面     
 * @author    yzg
 * @version   1.0
 * @date      2021.01.23 11:26:16
 * @warning
 * @copyright NanJing RenGu.
 * @note
 */
 
#pragma once

#include <QWidget>
#include "ui_taskreportswidget.h"

namespace Related {

	class TaskReportsWidget : public QWidget
	{
		Q_OBJECT

	public:
		TaskReportsWidget(QWidget *parent = Q_NULLPTR);
		~TaskReportsWidget();

	private:
		Ui::TaskReportsWidget ui;
	};

}//namespace Related 