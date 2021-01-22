/*!
 * @brief     系统登录页面
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
#include <base\selfwidget\ripwidget.h>
#include <base\util\widgetanimation.h>

#include "../net/protocol.h"

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
		bool eventFilter(QObject *watched, QEvent *event);

	signals:
		void switchToMainPage();

	private slots:
		void connectToServer();
		void respNetConnected(bool connected);
		void processUserLoginResponse(const UserLoginResponse & response);
		
		void showSystemSetting();
		
		void respSave();
		void respCancel();

	private:
		void init();
		void initConnect();
		void loadNetConfig();

	private:
		struct ConfigKey {
			QString m_netGroupId = "RemoteServer";
			QString m_remoteServerIp = "ServerIp";
			QString m_remoteServerDataPort = "ServerDataPort";
		};
	
	private:
		CustomWidgetContainer * m_container;
		QWidget * m_loginWidget;
		QLineEdit * m_userName;
		QLineEdit * m_password;
		Base::RIconButton * m_loginButt;

		Base::RIconButton * m_systemSetting;		/*!< 系统设置按钮 */
		QWidget * m_systemWidget;
		Base::RIPWidget * m_ipWidget;
		QLineEdit * m_portWidget;

		Base::WidgetAnimation m_animation;
	};

} //namespace Related 