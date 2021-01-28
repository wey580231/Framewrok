#include "iconbutton.h"

#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QDebug>

namespace Base {

	RIconButton::RIconButton(QWidget *parent) :QAbstractButton(parent),
		m_spacing(6), m_mouseEnter(false), m_colorChoose(Color_All), m_textVixible(true), m_iconLeftToText(true),
		m_mousePressed(false)
	{
		setIconSize(ICON_16);
		setContentsMargins(6, 5, 6, 5);
		m_borderRadius = { 2,2 };

		m_textFont = font();
		m_disableColor = QColor(0, 0, 0, 0);

		m_colorCollect.m_normalBackGroundColor = QColor(255, 0, 0, 0);
		m_colorCollect.m_checkedBackGroundColor = QColor(35, 188, 207, 100);
		m_colorCollect.m_hoverBackGroundColor = QColor(35, 188, 207, 100);

		m_colorCollect.m_normalBorderColor = QColor(118, 121, 124);
		m_colorCollect.m_checkedBorderColor = QColor(40, 206, 226);
		m_colorCollect.m_hoverBorderColor = QColor(40, 206, 226);

		m_colorCollect.m_normalTextColor = QColor(Qt::white);
		m_colorCollect.m_checkedTextColor = QColor(Qt::white);
		m_colorCollect.m_hoverTextColor = QColor(Qt::white);
	}

	void RIconButton::setCheckedIcon(const QIcon &icon)
	{
		m_checkedIcon = icon;
		update();
	}

	void RIconButton::setIconSize(RIconButton::IconSize type, QSize size)
	{
		switch (type) {
			case ICON_16: m_iconSize = QSize(16, 16); break;
			case ICON_24: m_iconSize = QSize(24, 24); break;
			case ICON_32: m_iconSize = QSize(32, 32); break;
			case ICON_48: m_iconSize = QSize(48, 48); break;
			case ICON_64: m_iconSize = QSize(64, 64); break;
			case ICON_custom: {
				if (size.isValid())
					m_iconSize = size;
				else
					m_iconSize = QSize(16, 16);
			}
							  break;
			default:
				break;
		}
	}

	void RIconButton::setTextFont(QFont & fontt)
	{
		m_textFont = fontt;
	}

	void RIconButton::setTextVisible(bool visible)
	{
		m_textVixible = visible;
	}

	void RIconButton::setBorderRadius(int radiusX /*= 0*/, int radiusY /*= 0*/)
	{
		m_borderRadius.first = radiusX;
		m_borderRadius.second = radiusY;
	}

	void RIconButton::setIconTextDirection(bool iconLeftToText /*= true*/)
	{
		m_iconLeftToText = iconLeftToText;
	}

	QSize RIconButton::sizeHint() const
	{
		return calcMiniumSize();
	}

	QSize RIconButton::minimumSizeHint() const
	{
		return calcMiniumSize();
	}

	void RIconButton::enableColor(RIconButton::ColorChoose choose, QColor color)
	{
		m_colorChoose |= choose;
		updateColor(choose, color);
	}

	void RIconButton::enableColors(ColorChooses chooses, QColor color /*= QColor(255, 0, 0, 0)*/)
	{
		int start = Color_NormalBackGround;
		while (start <= Color_HoverText) {
			if (chooses & start) {
				m_colorChoose &= start;
				updateColor((ColorChoose)start, color);
			}
			start <<= 1;
		}
	}

	void RIconButton::disableColor(RIconButton::ColorChoose choose)
	{
		m_colorChoose &= ~choose;
		updateColor(choose, m_disableColor);
	}

	void RIconButton::disableColors(ColorChooses chooses)
	{
		int start = Color_NormalBackGround;
		while (start <= Color_HoverText) {
			if (chooses & start) {
				m_colorChoose &= ~start;
				updateColor((ColorChoose)start, m_disableColor);
			}
			start <<= 1;
		}
	}

	void RIconButton::setHoverIcon(const QIcon & icon)
	{
		m_hoverIcon = icon;
		update();
	}

	void RIconButton::paintEvent(QPaintEvent *event)
	{
		Q_UNUSED(event)
			QPainter painter(this);

		int pressOffsetX = m_mousePressed ? 1 : 0;
		int pressOffsetY = m_mousePressed ? 1 : 0;

		//[1]绘制边框及背景
		painter.save();

		bool t_borderEnable = false;

		if (isChecked()) {
			painter.setBrush(QBrush(m_colorCollect.m_checkedBackGroundColor));
			painter.setPen(QPen(m_colorCollect.m_checkedBorderColor, 1));

			t_borderEnable = m_colorChoose.testFlag(Color_CheckedBorder);

		}
		else {
			if (m_mouseEnter) {
				painter.setBrush(QBrush(m_colorCollect.m_hoverBackGroundColor));
				painter.setPen(QPen(m_colorCollect.m_hoverBorderColor, 1));
				t_borderEnable = m_colorChoose.testFlag(Color_HoverBorder);
			}
			else {
				painter.setBrush(QBrush(m_colorCollect.m_normalBackGroundColor));
				painter.setPen(QPen(m_colorCollect.m_normalBorderColor, 1));
				t_borderEnable = m_colorChoose.testFlag(Color_NormalBorder);
			}
		}

		if (t_borderEnable) {
			QRect rec = rect();
			rec.adjust(1, 1, -1, -1);

			painter.drawRoundedRect(rec, m_borderRadius.first, m_borderRadius.second);
		}
		else {
			painter.drawRoundedRect(rect(), m_borderRadius.first, m_borderRadius.second);
		}

		painter.restore();

		QMargins margins = contentsMargins();

		QPoint startPoint;
		if (m_iconLeftToText) {
			startPoint = QPoint(margins.left(), margins.top());
		}
		else
		{
			startPoint = QPoint(width() - margins.right(), margins.top());
		}

		QSize minSize = calcMiniumSize();

		if (m_iconLeftToText) {
			//若控件尺寸大于最小绘制尺寸，则将其居中绘制
			if (width() > minSize.width()) {
				startPoint.setX(startPoint.x() + (width() - minSize.width()) / 2);
			}
			else {
				QSize contentSize = calcMiniumContentSize();
				if (width () > contentSize.width()) {
					startPoint.setX((width() - contentSize.width()) / 2);
				}
			}
		}
		else {
			if (width() > minSize.width())
				startPoint.setX(startPoint.x() - (width() - minSize.width()) / 2);
		}

		if (height() > minSize.height())
			startPoint.setY(startPoint.y() + (height() - minSize.height()) / 2);


		//[2]绘制图标
		QIcon qic = icon();

		if ( m_mouseEnter && !m_hoverIcon.isNull())
			qic = m_hoverIcon;
		
		if ((isChecked()) && !m_checkedIcon.isNull())
			qic = m_checkedIcon;

		if (!qic.isNull()) {
			QPixmap pic = qic.pixmap(m_iconSize, QIcon::Normal);
			int picY = (rect().height() - m_iconSize.height()) / 2;

			if (m_iconLeftToText) {
				painter.drawPixmap(QPoint(startPoint.x() + pressOffsetX, picY + pressOffsetY), pic);
				startPoint += QPoint(m_iconSize.width(), 0);
			}
			else
			{
				painter.drawPixmap(QPoint(startPoint.x() - m_iconSize.width() + pressOffsetX, picY + pressOffsetY), pic);
				startPoint -= QPoint(m_iconSize.width(), 0);
			}
		}

		if (!qic.isNull() && !text().isEmpty()) {
			if (m_iconLeftToText) {
				startPoint.setX(startPoint.x() + m_spacing);
			}
			else
			{
				startPoint.setX(startPoint.x() - m_spacing);
			}
		}

		//[3]绘制文字
		if (!text().isEmpty() && m_textVixible) {
			QFontMetrics fm(m_textFont);
			int minWidth = fm.width(text());
			int minHeight = fm.height();

			int txtY = (rect().height() - minHeight) / 2;

			QRect rect;
			if (m_iconLeftToText) {
				rect = QRect(startPoint.x() + pressOffsetX, txtY + pressOffsetY, minWidth, minHeight);
			}
			else
			{
				rect = QRect(startPoint.x() - minWidth + pressOffsetX, txtY, minWidth + pressOffsetY, minHeight);
			}

			painter.setFont(m_textFont);

			if (isChecked()) {
				painter.setPen(QPen(m_colorCollect.m_checkedTextColor, 1));
			}
			else {
				if (m_mouseEnter) {
					painter.setPen(QPen(m_colorCollect.m_hoverTextColor, 1));
				}
				else {
					painter.setPen(QPen(m_colorCollect.m_normalTextColor, 1));
				}
			}

			if (m_iconLeftToText) {
				painter.drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, text());
			}
			else
			{
				painter.drawText(rect, Qt::AlignRight | Qt::AlignVCenter, text());
			}
		}
	}

	void RIconButton::mousePressEvent(QMouseEvent * event)
	{
		m_mousePressed = true;
		update();

		QAbstractButton::mousePressEvent(event);
	}

	void RIconButton::mouseReleaseEvent(QMouseEvent * event)
	{
		m_mousePressed = false;
		update();

		QAbstractButton::mouseReleaseEvent(event);
	}

	void RIconButton::enterEvent(QEvent *event)
	{
		m_mouseEnter = true;
		update();
	}

	void RIconButton::leaveEvent(QEvent *event)
	{
		m_mouseEnter = false;
		update();
	}

	QSize RIconButton::calcMiniumSize() const
	{
		QMargins margins = contentsMargins();

		QPoint startPoint(margins.left(), margins.top());
		QIcon qic = icon();

		int m_maxY = 0;

		if (!qic.isNull()) {
			m_maxY = m_iconSize.height();
			startPoint += QPoint(m_iconSize.width(), 0);
		}

		if (!qic.isNull() && !text().isEmpty()) {
			startPoint.setX(startPoint.x() + m_spacing);
		}

		if (!text().isEmpty() && m_textVixible) {
			QFontMetrics fm(m_textFont);

			if (fm.height() > m_maxY)
				m_maxY = fm.height();

			startPoint += QPoint(fm.width(text()), 0);
		}

		return QSize(startPoint.x() + margins.right(), m_maxY + margins.top() + margins.bottom());
	}

	QSize RIconButton::calcMiniumContentSize() const
	{
		int theight = 0;
		int twidth = 0;

		QIcon qic = icon();
		if (!qic.isNull()) {
			theight = m_iconSize.height();
			twidth += m_iconSize.width();
		}

		if (!qic.isNull() && !text().isEmpty()) {
			twidth += m_spacing;
		}

		if (!text().isEmpty() && m_textVixible) {
			QFontMetrics fm(m_textFont);

			twidth += fm.width(text());

			if (fm.height() > theight)
				theight = fm.height();
		}

		return QSize(twidth, theight);
	}

	void RIconButton::updateColor(RIconButton::ColorChoose choose, QColor color)
	{
		switch (choose) {
			case Color_NormalBackGround:m_colorCollect.m_normalBackGroundColor = color; break;
			case Color_CheckedBackGround:m_colorCollect.m_checkedBackGroundColor = color; break;
			case Color_HoverBackground:m_colorCollect.m_hoverBackGroundColor = color; break;
			case Color_NormalBorder:m_colorCollect.m_normalBorderColor = color; break;
			case Color_CheckedBorder:m_colorCollect.m_checkedBorderColor = color; break;
			case Color_HoverBorder:m_colorCollect.m_hoverBorderColor = color; break;
			case Color_NormalText:m_colorCollect.m_normalTextColor = color; break;
			case Color_CheckedText:m_colorCollect.m_checkedTextColor = color; break;
			case Color_HoverText:m_colorCollect.m_hoverTextColor = color; break;
			default:break;
		}
	}

} //namespace Base
