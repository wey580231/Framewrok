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
#include <QCheckBox>
#include <QSpinBox>
#include <base/selfwidget/iconbutton.h>
#include <base\selfwidget\ripwidget.h>
#include <base\util\widgetanimation.h>

#include <commondefines/protocol.h>
#include <commondefines/structdefines.h>

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
		void netStateChanged(Datastruct::ConnectionType type,bool isConnected);

	private slots:
		void connectToServer();
		void reConnectServer();

		void respConnectResult(Datastruct::ConnectionType type, bool connected, QString errorInfo);
		void respNetConnected(Datastruct::ConnectionType type , bool connected);
		void respReconnResult(Datastruct::ConnectionType type, int times);
		void processUserLoginResponse(const Datastruct::UserLoginResponse & response);
		void processUserRegistResponse(const Datastruct::UserRegistResponse & response);
		
		void showSystemSetting();
		void showRegistWindow();
		
		void respSave();
		void respCancel();

		void respRegist();
		void respCancelRegist();

	private:
		void init();
		void initConnect();
		void loadNetConfig();
		void updateReconnConfig();
		void clearRegistInput();

	private:
		struct ConfigKey {
			QString m_netGroupId = "RemoteServer";

			QString m_remoteServerIp = "ServerIp";
			QString m_remoteServerDataPort = "DataServerPort";
			QString m_remoteServerFilePort = "FileServerPort";
			QString m_netAutoReconnect = "NetAutoReconnect";
			QString m_maxReconnTimes = "MaxReconnTimes";
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
		QLineEdit * m_dataPortWidget;
		QLineEdit * m_filePortWidget;
		QCheckBox * m_autoReconnet;
		QLineEdit * m_reconnectTimes;

		Base::RIconButton * m_userRegistSetting;	/*!< 用户注册按钮 */
		QWidget * m_registWidget;

		QLineEdit * m_registUserName;
		QLineEdit * m_registPassword1;
		QLineEdit * m_registPassword2;

		Base::WidgetAnimation m_animation;

		bool m_isLoginState;	/*!< 是否为登录状态(包括注册) */
		bool m_loginModel;		/*!< true:登录模式；false:注册模式 */
	};

} //namespace Related 