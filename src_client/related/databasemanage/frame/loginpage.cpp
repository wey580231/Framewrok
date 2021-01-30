#include "loginpage.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QResizeEvent>
#include <QDebug>

#include <base\util\rutil.h>
#include <base\selfwidget\rmessagebox.h>

#include "../customwidget/customwidgetcontainer.h"
#include "../net/datanetconnector.h"
#include "../net/signaldispatch.h"
#include "../utils/util.h"
#include "../global.h"

namespace Related {

	LoginPage::LoginPage(QWidget *parent)
		: QWidget(parent), m_loginModel(true), m_isLoginState(true), m_isReconnecting(false)
	{
		init();
		initConnect();
		loadNetConfig();

		//NOTE 默认提供自动重连机制，用户也可以手动连接
		DataNetConnector::instance()->setAutoReconnect(true);
	}

	LoginPage::~LoginPage()
	{

	}

	void LoginPage::resizeEvent(QResizeEvent *event)
	{
		QSize windowSize = event->size();

		int x = windowSize.width() - m_container->width() * 1.5;
		int y = (windowSize.height() - m_container->height()) / 2 ;
		m_container->move(x, y);
	}

	bool LoginPage::eventFilter(QObject *watched, QEvent *event)
	{
		if (watched == m_loginWidget) {
			if (event->type() == QEvent::Resize) {
				m_userRegistSetting->move(QPoint(m_loginWidget->width() / 2 - 5 - m_userRegistSetting->width(), m_loginWidget->height() - m_userRegistSetting->height() - 10));
				m_systemSetting->move(QPoint(m_loginWidget->width() / 2 + 5, m_loginWidget->height() - m_systemSetting->height() - 10));
			}
		}
		return QWidget::eventFilter(watched, event);
	}

	/*!
	 * @brief 尝试连接至远程数据、文件服务器
	 */
	void LoginPage::connectToServer()
	{
		START_WAIT

		if (!DataNetConnector::instance()->isConnected()) {
			if (DataNetConnector::instance()->connectTo(m_ipWidget->getIPString(), m_dataPortWidget->text().toInt())) {

			}
			else {
				END_WAIT
				Util::showWarning(this, QStringLiteral("连接服务器失败，请检查网络配置."));
			}
		}
		else {
			respNetConnected(true);
		}
	}

	/*! 
	 * @brief 响应用户点击重新连接网络
	 * @detials 避免用户多次点击
	 */
	void LoginPage::reConnectServer()
	{
		if (!DataNetConnector::instance()->isConnected()) {
			
			if (!m_isReconnecting) {
				m_isReconnecting = true;
				connectToServer();
			}
		}
	}

	/*!
	 * @brief 接收网络是否连接成功信号
	 * @param connected true:网络连接成功；false:网络连接失败
	 */
	void LoginPage::respNetConnected(bool connected)
	{
		m_isReconnecting = false;

		if (connected) {
			if (m_isLoginState) {
				if (m_loginModel) {
					Datastruct::UserLoginRequest request;
					request.m_name = m_userName->text();
					request.m_password = Base::RUtil::MD5(m_password->text());

					DataNetConnector::instance()->write(request);
				}
				else {
					Datastruct::UserRegistRequest request;
					request.m_name = m_registUserName->text();
					request.m_password = Base::RUtil::MD5(m_registPassword1->text());

					DataNetConnector::instance()->write(request);
				}			
			}
			else {
				END_WAIT
			}

			Global::G_Notify->information(QStringLiteral("网络已连接!"));

			emit netStateChanged(true);
		}
		else {
			END_WAIT
			emit netStateChanged(false);
			Util::showWarning(this, QStringLiteral("连接服务器失败，请检查网络配置."));
		}
	}

	/*!
	 * @brief 处理用户登录结果响应
	 * @param response 用户登录结果报文
	 */
	void LoginPage::processUserLoginResponse(const Datastruct::UserLoginResponse & response)
	{
		if (response.m_loginResult) {
			m_isLoginState = false;
			emit switchToMainPage();
			Global::G_UserEntity = response.m_userInfo;
		}
		else {
			Util::showWarning(this, Datastruct::G_StrError(response.m_errorCode));
		}
	}

	void LoginPage::processUserRegistResponse(const Datastruct::UserRegistResponse & response)
	{
		if (response.m_loginResult) {
			Util::showInformation(this, QStringLiteral("注册成功!"));
			respCancelRegist();
		}
		else {
			Util::showWarning(this, Datastruct::G_StrError(response.m_errorCode));
		}
	}

	void LoginPage::showSystemSetting()
	{
		m_systemWidget->setGeometry(QRect(QPoint(0, m_loginWidget->height()), m_loginWidget->size()));
		m_animation.addAnimation(m_systemWidget, QRect(QPoint(0, 0), m_loginWidget->size()), [&]() {
			loadNetConfig();
		});
		m_systemWidget->show();
	}

	void LoginPage::respSave()
	{
		ConfigKey ckey;
		Base::RUtil::setGlobalValue(ckey.m_netGroupId, ckey.m_remoteServerIp, m_ipWidget->getIPString());
		Base::RUtil::setGlobalValue(ckey.m_netGroupId, ckey.m_remoteServerDataPort, m_dataPortWidget->text());
		Base::RUtil::setGlobalValue(ckey.m_netGroupId, ckey.m_remoteServerFilePort, m_filePortWidget->text());

		respCancel();
	}

	void LoginPage::respCancel()
	{
		m_animation.addAnimation(m_systemWidget, QRect(QPoint(0, m_loginWidget->height()), m_loginWidget->size()), [&]() {
			m_systemWidget->hide();
		});
	}

	void LoginPage::showRegistWindow()
	{
		m_registWidget->setGeometry(QRect(QPoint(0, m_loginWidget->height()), m_loginWidget->size()));
		m_animation.addAnimation(m_registWidget, QRect(QPoint(0, 0), m_loginWidget->size()), [&]() {
			m_loginModel = false;
		});
		m_registWidget->show();
	}

	void LoginPage::respRegist()
	{
		if (m_registPassword1->text().isEmpty() || m_registPassword1->text().isEmpty()) {
			Util::showWarning(this, QStringLiteral("密码为空，请重新填写!"));
			return;
		}

		if (m_registPassword1->text() != m_registPassword2->text()) {
			Util::showWarning(this, QStringLiteral("两次密码不一致，请重新填写!"));
			return;
		}

		connectToServer();
	}

	void LoginPage::respCancelRegist()
	{
		m_animation.addAnimation(m_registWidget, QRect(QPoint(0, m_loginWidget->height()), m_loginWidget->size()), [&]() {
			clearRegistInput();

			m_loginModel = true;

			m_registWidget->hide();
		});
	}

	void LoginPage::init()
	{
		QWidget * mainWidget = new QWidget();
		mainWidget->setObjectName("Widget_LoginBackGround");

		m_loginWidget = new QWidget();
		m_loginWidget->installEventFilter(this);

		QLabel * titleLabel = new QLabel();
		titleLabel->setObjectName("Label_Login_TitleLabel");
		titleLabel->setAlignment(Qt::AlignCenter);
		titleLabel->setFixedHeight(30);
		titleLabel->setText(QStringLiteral("系统账户登录"));

		m_userName = new QLineEdit();
		m_userName->setMinimumHeight(25);
		m_userName->setPlaceholderText(QStringLiteral("请输入用户名"));
		m_userName->setText("root");

		m_password = new QLineEdit();
		m_password->setEchoMode(QLineEdit::Password);
		m_password->setMinimumHeight(25);
		m_password->setPlaceholderText(QStringLiteral("请输入密码"));
		m_password->setText("1234");

		m_loginButt = new Base::RIconButton();
		m_loginButt->setIconSize(Base::RIconButton::ICON_24);
		m_loginButt->setIcon(QIcon(WRAP_RESOURCE(登录)));
		m_loginButt->setText(QStringLiteral("登录"));
		connect(m_loginButt, SIGNAL(clicked()), this, SLOT(connectToServer()));

		m_userRegistSetting = new Base::RIconButton(m_loginWidget);
		m_userRegistSetting->disableColors(Base::RIconButton::Color_All);
		m_userRegistSetting->enableColors(Base::RIconButton::Color_NormalText, Qt::white);
		m_userRegistSetting->enableColors(Base::RIconButton::Color_HoverText, Qt::red);
		m_userRegistSetting->setText(QStringLiteral("注册账号"));
		connect(m_userRegistSetting, SIGNAL(clicked()), this, SLOT(showRegistWindow()));


		m_systemSetting = new Base::RIconButton(m_loginWidget);
		m_systemSetting->disableColors(Base::RIconButton::Color_All);
		m_systemSetting->enableColors(Base::RIconButton::Color_NormalText, Qt::white);
		m_systemSetting->enableColors(Base::RIconButton::Color_HoverText, Qt::red);
		m_systemSetting->setText(QStringLiteral("系统设置"));
		connect(m_systemSetting, SIGNAL(clicked()), this, SLOT(showSystemSetting()));

		QVBoxLayout * vlayout = new QVBoxLayout();
		vlayout->addWidget(titleLabel);
		vlayout->addWidget(m_userName);
		vlayout->addWidget(m_password);
		vlayout->addWidget(m_loginButt);

		m_loginWidget->setLayout(vlayout);

		int fixedWidth = 80;
		auto createLabel = [&](QString text) {
			QLabel * label = new QLabel();
			label->setFixedWidth(fixedWidth);
			label->setText(text);
			label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
			return label;
		};

		QSize fixSize(70, 25);
		auto createButton = [&](QString text, const char * pslot) {
			Base::RIconButton * butt = new Base::RIconButton();
			butt->setText(text);
			butt->setFixedSize(fixSize);
			connect(butt, SIGNAL(clicked()), this, pslot);
			return butt;
		};

		//系统设置页面
		{
			m_systemWidget = new QWidget(m_loginWidget);
			m_systemWidget->setStyleSheet("background-color:rgba(0,77,136, 230)");

			QLabel * remoteIp = createLabel(QStringLiteral("服务器地址:"));
			QLabel * remotePort = createLabel(QStringLiteral("端口号:"));

			m_ipWidget = new Base::RIPWidget();
			m_dataPortWidget = new QLineEdit();
			m_filePortWidget = new QLineEdit();

			Base::RIconButton * saveButt = createButton(QStringLiteral("更新"), SLOT(respSave()));
			Base::RIconButton * cancelButt = createButton(QStringLiteral("取消"), SLOT(respCancel()));

			QWidget * toolContainer = new QWidget();
			toolContainer->setMinimumHeight(40);
			QHBoxLayout * toolLayout = new QHBoxLayout();
			toolLayout->addStretch(1);
			toolLayout->addWidget(saveButt);
			toolLayout->addWidget(cancelButt);
			toolLayout->addStretch(1);
			toolContainer->setLayout(toolLayout);

			QGridLayout * slayout = new QGridLayout();
			slayout->setVerticalSpacing(15);

			slayout->setRowStretch(0, 1);
			slayout->addWidget(remoteIp, 1, 0, 1, 1);
			slayout->addWidget(m_ipWidget, 1, 1, 1, 1);
			slayout->addWidget(remotePort, 2, 0, 1, 1);
			slayout->addWidget(m_dataPortWidget, 2, 1, 1, 1);
			slayout->addWidget(m_filePortWidget, 3, 1, 1, 1);
			slayout->setRowStretch(4, 1);

			slayout->addWidget(toolContainer, 5, 0, 1, 2);

			m_systemWidget->setLayout(slayout);
			m_systemWidget->hide();
		}

		//用户注册页面
		{
			m_registWidget = new QWidget(m_loginWidget);
			m_registWidget->setStyleSheet("background-color:rgba(0,77,136, 230)");

			QLabel * registUserName = createLabel(QStringLiteral("用户名:"));
			QLabel * password1 = createLabel(QStringLiteral("密码:"));
			QLabel * password2 = createLabel(QStringLiteral("确认密码:"));

			m_registUserName = new QLineEdit();

			m_registPassword1 = new QLineEdit();
			m_registPassword1->setEchoMode(QLineEdit::Password);

			m_registPassword2 = new QLineEdit();
			m_registPassword2->setEchoMode(QLineEdit::Password);

			Base::RIconButton * registButt = createButton(QStringLiteral("注册"), SLOT(respRegist()));
			Base::RIconButton * cancelButt = createButton(QStringLiteral("取消"), SLOT(respCancelRegist()));

			QWidget * toolContainer = new QWidget();
			toolContainer->setMinimumHeight(40);
			QHBoxLayout * toolLayout = new QHBoxLayout();
			toolLayout->addStretch(1);
			toolLayout->addWidget(registButt);
			toolLayout->addWidget(cancelButt);
			toolLayout->addStretch(1);
			toolContainer->setLayout(toolLayout);

			QGridLayout * slayout = new QGridLayout();
			slayout->setVerticalSpacing(15);

			slayout->setRowStretch(0, 1);
			slayout->addWidget(registUserName, 1, 0, 1, 1);
			slayout->addWidget(m_registUserName, 1, 1, 1, 1);
			slayout->addWidget(password1, 2, 0, 1, 1);
			slayout->addWidget(m_registPassword1, 2, 1, 1, 1);
			slayout->addWidget(password2, 3, 0, 1, 1);
			slayout->addWidget(m_registPassword2, 3, 1, 1, 1);
			slayout->setRowStretch(4, 1);

			slayout->addWidget(toolContainer, 5, 0, 1, 2);

			m_registWidget->setLayout(slayout);
			m_registWidget->hide();
		}

		m_container = new CustomWidgetContainer(mainWidget);
		m_container->setFixedSize(360, 420);
		m_container->setContent(m_loginWidget);
		m_container->show();

		QHBoxLayout * layout = new QHBoxLayout();
		layout->setContentsMargins(0, 0, 0, 0);
		layout->addWidget(mainWidget);
		setLayout(layout);
	}

	void LoginPage::initConnect()
	{
		connect(DataNetConnector::instance(), SIGNAL(netConnected(bool)), this, SLOT(respNetConnected(bool)));
		connect(SignalDispatch::instance(), SIGNAL(respUserLoginResponse(const Datastruct::UserLoginResponse &)), this, SLOT(processUserLoginResponse(const Datastruct::UserLoginResponse &)));
		connect(SignalDispatch::instance(), SIGNAL(respUserRegistResponse(const Datastruct::UserRegistResponse &)), this, SLOT(processUserRegistResponse(const Datastruct::UserRegistResponse &)));
	}

	void LoginPage::loadNetConfig()
	{
		ConfigKey ckey;

		m_ipWidget->setIP(Base::RUtil::getGlobalValue(ckey.m_netGroupId, ckey.m_remoteServerIp, "127.0.0.1").toString());
		m_dataPortWidget->setText(Base::RUtil::getGlobalValue(ckey.m_netGroupId, ckey.m_remoteServerDataPort, "8888").toString());
		m_filePortWidget->setText(Base::RUtil::getGlobalValue(ckey.m_netGroupId, ckey.m_remoteServerFilePort, "9999").toString());
	}

	void LoginPage::clearRegistInput()
	{
		m_registUserName->clear();
		m_registPassword1->clear();
		m_registPassword2->clear();
	}

} //namespace Related 