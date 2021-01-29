#include "notify.h"

#include <QPropertyAnimation>
#include <QHBoxLayout>
#include <QTimer>
#include <QMouseEvent>

namespace Base {

	Notify::Notify(int displayTime,QWidget *parent)
		: QWidget(parent), m_displayTime(displayTime)
	{
		init();
	}

	Notify::~Notify()
	{
	}

	void Notify::setIcon(const QString &value)
	{
		m_icon = value;
	}

	void Notify::setTitle(const QString &value)
	{
		m_title = value;
	}

	void Notify::setContent(const QString &value)
	{
		m_body = value;
	}

	void Notify::showGriant()
	{
		this->show();

		m_titleLabel->setText(m_title);

		QPixmap tempPix = QPixmap(m_icon);
		tempPix = tempPix.scaled(QSize(30, 30), Qt::KeepAspectRatio);
		m_iconLabel->setPixmap(tempPix);

		m_backgroundLabel->setFixedSize(this->size());
		m_closeBtn->move(m_backgroundLabel->width() - m_closeBtn->width(), 0);

		// 超过长度省略号
		QFontMetrics elidfont(m_bodyLabel->font());
		QString text = elidfont.elidedText(m_body, Qt::ElideRight, m_bodyLabel->width() - 5);
		m_bodyLabel->setText(text);

		QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity", this);
		animation->setStartValue(0);
		animation->setEndValue(1);
		animation->setDuration(200);
		animation->start();

		connect(animation, &QPropertyAnimation::finished, this, [animation, this]() {
			animation->deleteLater();
			QTimer::singleShot(m_displayTime, this, [this]() {
				this->hideGriant();
			});
		});
	}

	void Notify::mousePressEvent(QMouseEvent * event)
	{
		if (event->button() == Qt::LeftButton) {
			hideGriant();
		}
	}

	void Notify::init()
	{
		this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::Tool | Qt::WindowStaysOnTopHint);
		this->setAttribute(Qt::WA_NoSystemBackground, true);
		this->setAttribute(Qt::WA_TranslucentBackground, true);

		m_backgroundLabel = new QWidget(this);
		m_backgroundLabel->move(0, 0);
		m_backgroundLabel->setObjectName("mainWidget");

		QHBoxLayout * mainLayout = new QHBoxLayout(m_backgroundLabel);
		QVBoxLayout * contentLayout = new QVBoxLayout();

		m_iconLabel = new QLabel(m_backgroundLabel);
		m_iconLabel->setFixedWidth(40);
		m_iconLabel->setAlignment(Qt::AlignCenter);

		m_titleLabel = new QLabel(m_backgroundLabel);
		m_titleLabel->setObjectName("notify-title");

		m_bodyLabel = new QLabel(m_backgroundLabel);
		m_bodyLabel->setObjectName("notify-body");
		QFont font = m_bodyLabel->font();
		font.setPixelSize(12);
		m_bodyLabel->setFont(font);

		contentLayout->addWidget(m_titleLabel);
		contentLayout->addWidget(m_bodyLabel);

		mainLayout->addWidget(m_iconLabel);
		mainLayout->addSpacing(5);
		mainLayout->addLayout(contentLayout);

		m_closeBtn = new RIconButton(m_backgroundLabel);
		m_closeBtn->setBorderRadius();
		m_closeBtn->disableColors(RIconButton::Color_NormalBorder | RIconButton::Color_HoverBorder);
		m_closeBtn->enableColors(RIconButton::Color_NormalBackGround, QColor(35, 113, 173, 240));
		m_closeBtn->enableColors(RIconButton::Color_HoverBackground, Qt::red);
		m_closeBtn->setIcon(QIcon(QStringLiteral(":/QYBlue/resource/qyblue/关闭.png")));
		m_closeBtn->setFixedSize(24, 24);

		connect(m_closeBtn, &QPushButton::clicked, this, [this] {
			Q_EMIT disappeared();
		});
	}

	/*!
	 * @brief 透明度逐渐减少隐藏
	 */
	void Notify::hideGriant()
	{
		QPropertyAnimation * animation = new QPropertyAnimation(this, "windowOpacity", this);
		animation->setStartValue(this->windowOpacity());
		animation->setEndValue(0);
		animation->setDuration(200);
		animation->start();

		connect(animation, &QPropertyAnimation::finished, this, [animation, this]() {
			this->hide();
			animation->deleteLater();
			emit disappeared();
		});
	}

} //namespace Base 