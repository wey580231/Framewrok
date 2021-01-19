/*!
 * @brief     ÏµÍ³µÇÂ¼Ò³Ãæ
 * @author    wey
 * @version   1.0
 * @date      2021.01.16 17:27:14
 * @warning
 * @copyright NanJing RenGu.
 * @note
 */
#pragma once

#include <QWidget>
#include <QLineEdit>
#include <base/selfwidget/iconbutton.h>

namespace Related {

	class CustomWidgetContainer;

	class LoginPage : public QWidget
	{
		Q_OBJECT

	public:
		LoginPage(QWidget *parent = nullptr);
		~LoginPage();

	protected:
		void resizeEvent(QResizeEvent *event);

	signals:
		void switchToMainPage();

	private slots:
		void respLogin();

	private:
		void init();

	private:
		CustomWidgetContainer * m_container;
		QLineEdit * m_userName;
		QLineEdit * m_password;
		Base::RIconButton * m_loginButt;
	};

} //namespace Related 