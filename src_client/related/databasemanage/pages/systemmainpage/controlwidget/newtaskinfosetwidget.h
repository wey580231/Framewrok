/*!
 * @brief     ���������Ϣ���ô���   
 * @author    yzg
 * @version   1.0
 * @date      2021.01.22 19:13:13
 * @warning
 * @copyright NanJing RenGu.
 * @note
 */
#pragma once

#include <QWidget>
#include <QDateTime>

#include "ui_newtaskinfosetwidget.h"

#include <base/util/rutil.h>

#include "../../../datastruct.h"

namespace Related {

	class NewTaskInfoSetWidget : public QWidget
	{
		Q_OBJECT

	public:
		NewTaskInfoSetWidget(QWidget *parent = Q_NULLPTR);
		~NewTaskInfoSetWidget();

		void setTaskBaseInfo(TaskBaseInfo info);

		TaskBaseInfo getTaskBaseInfo();

	private:
		void init();

	private:
		Ui::NewTaskInfoSetWidget ui;
		TaskBaseInfo m_taskBaseInfo;			/*!< ���������Ϣ */
	};
}
