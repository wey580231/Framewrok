/*!
 * @brief     试验报告编辑窗口
 * @details 
 * @author    yzg
 * @version   1.0
 * @date      2021.02.01 16:39:31
 * @warning
 * @copyright NanJing RenGu.
 * @note
 */
 #pragma once

#include <QWidget>
#include "ui_taskreporteditpage.h"

class TaskReportEditPage : public QWidget
{
	Q_OBJECT

public:
	TaskReportEditPage(QWidget *parent = Q_NULLPTR);
	~TaskReportEditPage();

private:
	Ui::TaskReportEditPage ui;
};
