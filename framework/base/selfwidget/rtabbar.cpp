#include "rtabbar.h"

#include <QPainter>
#include <QLinearGradient>

namespace Base {

#define TAB_BUTT_FIXED_WIDTH 150
#define TAB_BUTT_FIXED_HEIGHT 35

	RTabButton::RTabButton(QString text, QWidget* parent) :QAbstractButton(parent), m_bMouseIn(false)
	{
		setText(text);

		m_selectedTextColor = QColor(15, 242, 246);
		m_normalTextColor = QColor(241, 241, 241, 150);
		m_disableTextColor = QColor(241, 241, 241, 100);

		setCheckable(true);
		setFixedWidth(TAB_BUTT_FIXED_WIDTH);
		setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
	}

	RTabButton::~RTabButton()
	{

	}

	void RTabButton::paintEvent(QPaintEvent* event)
	{
		QPainter painter(this);

		QFont font = painter.font();
		font.setFamily(QStringLiteral("微软雅黑"));

		if (isEnabled()) {
			if (m_bMouseIn || isChecked()) {
				painter.setPen(QPen(m_selectedTextColor));
				font.setPixelSize(17);

				painter.save();

				double rectHeight = rect().height() * 0.05;
				rectHeight = rectHeight < 2 ? 2 : rectHeight;

				QLinearGradient linear(0,0, rect().width(), rectHeight);
				linear.setColorAt(0, QColor(0,174,239));
				linear.setColorAt(1, QColor(15,242,246));
				painter.setBrush(linear);

				painter.setPen(Qt::NoPen);
				painter.drawRect(QRect(0, rect().height() - rectHeight, width(), rectHeight));

				painter.restore();
			}
			else {
				painter.setPen(QPen(m_normalTextColor));
				font.setPixelSize(14);
				font.setBold(false);
			}
		}
		else {
			painter.setPen(QPen(m_disableTextColor));
			font.setPixelSize(15);
			font.setBold(false);
		}

		painter.setFont(font);

		painter.drawText(rect(), Qt::AlignCenter, text());

	}

	void RTabButton::enterEvent(QEvent* event)
	{
		m_bMouseIn = true;
		update();
	}

	void RTabButton::leaveEvent(QEvent* event)
	{
		m_bMouseIn = false;
		update();
	}

	RTabBar::RTabBar(QWidget* parent) :QWidget(parent), m_buttGroup(nullptr), m_tabAlign(AlignLeft)
	{
		init();
	}

	RTabBar::~RTabBar()
	{

	}

	void RTabBar::addTabButton(RTabButton* butt, int index)
	{
		QHBoxLayout * layout = dynamic_cast<QHBoxLayout *>(m_tabContainer->layout());
		if (layout && butt) {
			layout->addWidget(butt);

			if (m_buttGroup == nullptr) {
				m_buttGroup = new QButtonGroup();
				connect(m_buttGroup, SIGNAL(buttonClicked(int)), this, SLOT(respTabChoosed(int)));
			}

			m_buttGroup->addButton(butt, index);
			m_buttList.append(butt);

			if (m_buttGroup->buttons().size() == 1) {
				butt->setChecked(true);
			}
		}
	}

	void RTabBar::addTabButton(QString tabName, int index)
	{
		if (tabName.isEmpty())
			return;

		RTabButton * tabButt = new RTabButton(tabName);
		addTabButton(tabButt, index);
	}

	/*!
	 * @brief 设置tab按钮是否可用
	 * @param[in] index 按钮索引
	 * @param[in] flag  true:有效；false:无效
	 */
	void RTabBar::setTabButtonEnabled(int index, bool flag)
	{
		QAbstractButton * butt = m_buttGroup->button(index);
		if (butt != nullptr) {
			butt->setEnabled(flag);
		}
	}

	/*!
	 * @brief 设置指定索引被选中
	 * @param index 最新tab索引
	 * @param indexChangeSignal true:发出索引变换信号，false:不发出信号
	 */
	void RTabBar::setCurrentIndex(int index, bool indexChangeSignal)
	{
		if (index >= 0 && index < m_buttList.size()) {
			m_buttList.at(index)->setChecked(true);

			if (indexChangeSignal)
				respTabChoosed(index);
		}
	}

	int RTabBar::currentIndex()
	{
		return m_buttList.indexOf(m_buttGroup->checkedButton());
	}

	void RTabBar::paintEvent(QPaintEvent *event)
	{
		if (m_buttList.size() > 0) {
			QPainter painter(this);

			QRect buttRect = m_buttList.first()->rect();

			QPoint bottLeft = m_buttList.first()->mapTo(this, buttRect.bottomLeft());
			
			painter.setPen(QColor(255, 255, 255, 50));
			painter.drawLine(QPoint(0, bottLeft.y()),QPoint(width(), bottLeft.y()));
		}
	}

	/*!
	 * @brief 设置tab对齐显示
	 * @param[in] align 新对其方式
	 */
	void RTabBar::setTabAlignment(TabAlignment align)
	{
		m_tabAlign = align;

		for (int i = m_mainLayout->count() - 1; i >= 0; i--)
			m_mainLayout->removeItem(m_mainLayout->itemAt(i));

		if (m_tabAlign == AlignLeft) {
			m_mainLayout->addWidget(m_tabContainer);
			m_mainLayout->addStretch(1);
		}
		else if (m_tabAlign == AlignCenter) {
			m_mainLayout->addStretch(1);
			m_mainLayout->addWidget(m_tabContainer);
			m_mainLayout->addStretch(1);
		}
		else if (m_tabAlign == AlignRight) {
			m_mainLayout->addStretch(1);
			m_mainLayout->addWidget(m_tabContainer);
		}
	}

	void RTabBar::respTabChoosed(int index)
	{
		emit currentIndexChanged(index);
	}

	void RTabBar::init()
	{
		QWidget * mainWidget = new QWidget();
		mainWidget->setObjectName("RCustomBarContainer");

		m_mainLayout = new QHBoxLayout();
		m_mainLayout->setContentsMargins(1, 1, 1, 1);

		mainWidget->setLayout(m_mainLayout);

		m_tabContainer = new QWidget();

		QHBoxLayout * tabContainer = new QHBoxLayout();
		tabContainer->setContentsMargins(9, 9, 9, 0);
		m_tabContainer->setLayout(tabContainer);

		setTabAlignment(m_tabAlign);

		QHBoxLayout * layout = new QHBoxLayout();
		layout->setContentsMargins(0, 0, 0, 0);
		layout->addWidget(mainWidget);
		setLayout(layout);
	}

} //namespace Base
