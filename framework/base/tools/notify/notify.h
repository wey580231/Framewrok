/*!
 * @brief     单个通知窗口，NotifyManger调用   
 * @author    wey
 * @version   1.0
 * @date      2021.01.29 16:25:48
 * @warning
 * @copyright NanJing RenGu.
 * @note
 */
 #pragma once

#include <QWidget>
#include <QLabel>
#include <QPushButton>

#include "../../selfwidget/iconbutton.h"

namespace Base {

	class Notify : public QWidget
	{
		Q_OBJECT

	public:
		Notify(int displayTime , QWidget *parent = nullptr);
		~Notify();

		void setIcon(const QString &value);
		void setTitle(const QString &value);
		void setContent(const QString &value);

		void showGriant();

	signals:
		void disappeared();

	protected:
		void mousePressEvent(QMouseEvent *event);

	private:
		void init();
		void hideGriant();

	private:
		int m_displayTime;			/*!< 通知显示时长 */

		QString m_icon;
		QString m_title;
		QString m_body;

		QWidget * m_backgroundLabel;
		QLabel * m_iconLabel;
		QLabel * m_titleLabel;
		QLabel * m_bodyLabel;

		RIconButton * m_closeBtn;
	};

} //namespace Base 