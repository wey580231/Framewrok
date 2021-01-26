/*!
 * @brief     任务基本信息设置窗口   
 * @author    yzg
 * @version   1.0
 * @date      2021.01.22 19:13:13
 * @warning
 * @copyright NanJing RenGu.
 * @note
 */
#pragma once

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "ui_newtaskinfosetwidget.h"

#include <base/util/rutil.h>

#include "../datastruct.h"

namespace Related {

	class NewTaskInfoSetWidget : public QWidget
	{
		Q_OBJECT

	public:
		NewTaskInfoSetWidget(QWidget *parent = Q_NULLPTR);
		~NewTaskInfoSetWidget();

		TaskBaseInfo getTaskBaseInfo();

	private:
		void init();

	private:
		Ui::NewTaskInfoSetWidget ui;
		TaskBaseInfo m_taskBaseInfo;			/*!< 任务基本信息 */
	};
}
