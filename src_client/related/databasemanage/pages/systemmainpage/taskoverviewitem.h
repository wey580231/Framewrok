/*!
 * @brief     单个任务概览信息   
 * @author    wey
 * @version   1.0
 * @date      2021.01.19 18:36:10
 * @warning
 * @copyright NanJing RenGu.
 * @note
 */
 #pragma once

#include <QWidget>
#include <QDateTime>
namespace Ui { class TaskOverViewItem; };

#include <base\util\widgetanimation.h>
#include <base/selfwidget/iconbutton.h>

#include <commondefines/structdefines.h>

namespace Related {

	class TaskOverViewItem : public QWidget
	{
		Q_OBJECT

	public:
		TaskOverViewItem(QWidget *parent = Q_NULLPTR);
		~TaskOverViewItem();

		void setTaskBaseInfo(Datastruct::TaskEntityData data);

	protected:
		bool eventFilter(QObject *watched, QEvent *event);

	signals:
		void openTask(QString id);
		void deleteTask(QString id);

	private:
		void init();

	private slots:
		void respButtPressed();

	private:
		/*!
		 * @brief 按钮类型
		 * @details 
		 */
		enum ButtType {
			Butt_Open,
			Butt_Delete
		};

	private:
		Ui::TaskOverViewItem *ui;

		QWidget * m_toolButtWidget;
		Base::WidgetAnimation m_widgetAnimation;
		Base::RIconButton * m_openTaskButt;
		Base::RIconButton * m_deleteTaskButt;

		Datastruct::TaskEntityData m_taskData;
	};

}//namespace Related 
