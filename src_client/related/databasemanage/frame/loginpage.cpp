#include "loginpage.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QResizeEvent>
#include <QDebug>

#include <base\util\rutil.h>
#include <base\selfwidget\rmessagebox.h>

#include "customwidget/customwidgetcontainer.h"
#include "../net/netconnector.h"
#include "../net/signaldispatch.h"
#include "../utils/util.h"

namespace Related {

	LoginPage::LoginPage(QWidget *parent)
		: QWidget(parent)
	{
		init();
		initConnect();
		loadNetConfig();
	}

	LoginPage::~LoginPage()
	{

	}

	void LoginPage::resizeEvent(QResizeEvent *event)
	{
		QSize windowSize = event->size();

		int x = (windowSize.width() - m_container->width()) / 2;
		int y = (windowSize.height() - m_container->height()) / 2 - 20;
		m_container->move(x, y);

	}

	bool LoginPage::eventFilter(QObject *watched, QEvent *event)
	{
		if (watched == m_loginWidget) {
			if (event->type() == QEvent::Resize) {
				m_systemSetting->move(QPoint((m_loginWidget->width() - m_systemSetting->width()) / 2, m_loginWidget->height() - m_systemSetting->height() - 10));
			}
		}
		return QWidget::eventFilter(watched, event);
	}

	/*!
	 * @brief 尝试连接至远程服务器
	 */
	void LoginPage::connectToServer()
	{
		if (!NetConnector::instance()->isConnected()) {
			if (NetConnector::instance()->connectTo(m_ipWidget->getIPString(), m_portWidget->text().toInt())) {

			}
			else {
				//TODO 提示错误
			}
		}
		else {
			respNetConnected(true);
		}
	}

	/*!
	 * @brief 接收网络是否连接成功信号
	 * @param connected true:网络连接成功；false:网络连接失败
	 */
	void LoginPage::respNetConnected(bool connected)
	{
		if (connected) {
			Datastruct::UserLoginRequest request;
			request.m_name = m_userName->text();
			request.m_password = Base::RUtil::MD5(m_password->text());

			NetConnector::instance()->write(request);
		}
		else {
			//TODO 提示网络连接失败
			qDebug() << "Net connect error!";
		}
	}

	/*!
	 * @brief 处理用户登录结果响应
	 * @param response 用户登录结果报文
	 */
	void LoginPage::processUserLoginResponse(const Datastruct::UserLoginResponse & response)
	{
		if (response.m_loginResult) {
			emit switchToMainPage();
			qDebug() << response.m_userInfo.id;
		}
		else {
			Util::showWarning(this, response.m_errorInfo);
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
		Base::RUtil::setGlobalValue(ckey.m_netGroupId, ckey.m_remoteServerDataPort, m_portWidget->text());

		respCancel();
	}

	void LoginPage::respCancel()
	{
		m_animation.addAnimation(m_systemWidget, QRect(QPoint(0, m_loginWidget->height()), m_loginWidget->size()), [&]() {
			m_systemWidget->hide();
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
		titleLabel->setFixedHeight(40);
		titleLabel->setText(QStringLiteral("某水声数据管理软件"));

		m_userName = new QLineEdit();
		m_userName->setMinimumHeight(25);
		m_userName->setPlaceholderText(QStringLiteral("请输入用户名"));
		m_userName->setText("root");

		m_password = new QLineEdit();
		m_password->setEchoMode(QLineEdit::Password);
		m_password->setMinimumHeight(25);
		m_password->setPlaceholderText(QStringLiteral("请输入密码"));
		m_password->setText("root");

		m_loginButt = new Base::RIconButton();
		m_loginButt->setIconSize(Base::RIconButton::ICON_24);
		m_loginButt->setIcon(QIcon(QStringLiteral(":/QYBlue/resource/qyblue/登录.png")));
		m_loginButt->setText(QStringLiteral("登录"));
		connect(m_loginButt, SIGNAL(pressed()), this, SLOT(connectToServer()));

		m_systemSetting = new Base::RIconButton(m_loginWidget);
		m_systemSetting->disableColors(Base::RIconButton::Color_All);
		m_systemSetting->enableColors(Base::RIconButton::Color_NormalText, Qt::white);
		m_systemSetting->enableColors(Base::RIconButton::Color_HoverText, Qt::red);
		m_systemSetting->setText(QStringLiteral("系统设置"));
		connect(m_systemSetting, SIGNAL(pressed()), this, SLOT(showSystemSetting()));

		QVBoxLayout * vlayout = new QVBoxLayout();
		vlayout->addWidget(titleLabel);
		vlayout->addWidget(m_userName);
		vlayout->addWidget(m_password);
		vlayout->addWidget(m_loginButt);

		m_loginWidget->setLayout(vlayout);

		//系统设置页面
		{
			m_systemWidget = new QWidget(m_loginWidget);
			m_systemWidget->setStyleSheet("background-color:rgba(0,77,136, 210)");

			int fixedWidth = 80;

			QLabel * remoteIp = new QLabel();
			remoteIp->setFixedWidth(fixedWidth);
			remoteIp->setText(QStringLiteral("服务器地址:"));
			remoteIp->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

			m_ipWidget = new Base::RIPWidget();

			QLabel * remotePort = new QLabel();
			remotePort->setFixedWidth(fixedWidth);
			remotePort->setText(QStringLiteral("端口号:"));
			remotePort->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

			m_portWidget = new QLineEdit();

			QSize fixSize(70, 25);
			Base::RIconButton * saveButt = new Base::RIconButton();
			saveButt->setText(QStringLiteral("更新"));
			saveButt->setFixedSize(fixSize);
			connect(saveButt, SIGNAL(pressed()), this, SLOT(respSave()));

			Base::RIconButton * cancelButt = new Base::RIconButton();
			cancelButt->setText(QStringLiteral("取消"));
			cancelButt->setFixedSize(fixSize);
			connect(cancelButt, SIGNAL(pressed()), this, SLOT(respCancel()));

			QWidget * toolContainer = new QWidget();
			toolContainer->setMinimumHeight(40);
			QHBoxLayout * toolLayout = new QHBoxLayout();
			toolLayout->addStretch(1);
			toolLayout->addWidget(saveButt);
			toolLayout->addWidget(cancelButt);
			toolLayout->addStretch(1);
			toolContainer->setLayout(toolLayout);

			QGridLayout * slayout = new QGridLayout();
			slayout->setVerticalSpacing(10);

			slayout->setRowStretch(0, 1);
			slayout->addWidget(remoteIp, 1, 0, 1, 1);
			slayout->addWidget(m_ipWidget, 1, 1, 1, 1);
			slayout->addWidget(remotePort, 2, 0, 1, 1);
			slayout->addWidget(m_portWidget, 2, 1, 1, 1);
			slayout->setRowStretch(3, 1);

			slayout->addWidget(toolContainer, 4, 0, 1, 2);

			m_systemWidget->setLayout(slayout);
			m_systemWidget->hide();
		}

		m_container = new CustomWidgetContainer(mainWidget);
		m_container->setFixedSize(380, 460);
		m_container->setContent(m_loginWidget);
		m_container->show();

		QHBoxLayout * layout = new QHBoxLayout();
		layout->setContentsMargins(0, 0, 0, 0);
		layout->addWidget(mainWidget);
		setLayout(layout);
	}

	void LoginPage::initConnect()
	{
		connect(NetConnector::instance(), SIGNAL(netConnected(bool)), this, SLOT(respNetConnected(bool)));
		connect(SignalDispatch::instance(), SIGNAL(respUserLoginResponse(const Datastruct::UserLoginResponse &)), this, SLOT(processUserLoginResponse(const Datastruct::UserLoginResponse &)));
	}

	void LoginPage::loadNetConfig()
	{
		ConfigKey ckey;

		m_ipWidget->setIP(Base::RUtil::getGlobalValue(ckey.m_netGroupId, ckey.m_remoteServerIp, "127.0.0.1").toString());
		m_portWidget->setText(Base::RUtil::getGlobalValue(ckey.m_netGroupId, ckey.m_remoteServerDataPort, "8080").toString());
	}

} //namespace Related 