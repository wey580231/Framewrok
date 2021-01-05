#include "flowcontrol.h"

#include <QMouseEvent>
#include <QAction>
#include <QPainter>

#include "../util/rutil.h"

namespace Base {

	FlowControl::FlowControl(QWidget *parent) :QWidget(parent), m_menu(nullptr), m_parent(nullptr)
	{

	}

	void FlowControl::addAction(QAction *action)
	{
		if (m_menu == nullptr)
			m_menu = new QMenu();
		m_menu->addAction(action);
	}

	void FlowControl::mousePressEvent(QMouseEvent *event)
	{
		if (event->button() == Qt::LeftButton) {
			m_globalMouseStartPoint = event->globalPos();
			m_b_leftButtonPressed = true;
			setCursor(Qt::SizeAllCursor);
		}
		QWidget::mousePressEvent(event);
	}

	void FlowControl::mouseMoveEvent(QMouseEvent *event)
	{
		if (m_b_leftButtonPressed)
		{
			QPoint ptemp = event->globalPos() - m_globalMouseStartPoint;

			if (m_parent)
				updateParentPos(ptemp);
			else
				emit posDiff(ptemp);

			m_globalMouseStartPoint = event->globalPos();
		}
	}

	void FlowControl::mouseReleaseEvent(QMouseEvent *event)
	{
		Q_UNUSED(event)
			m_b_leftButtonPressed = false;
		setCursor(Qt::ArrowCursor);
	}

	void FlowControl::paintEvent(QPaintEvent *event)
	{
		Q_UNUSED(event)
			QPainter painter(this);
		QColor yellowColor(255, 195, 0, 245);
		painter.setBrush(yellowColor);
		painter.setPen(Qt::NoPen);
		painter.drawRect(rect());
	}

	void FlowControl::contextMenuEvent(QContextMenuEvent *event)
	{
		if (m_menu)
			m_menu->exec(event->globalPos());
	}

	void FlowControl::enterEvent(QEvent *event)
	{
		setCursor(Qt::SizeAllCursor);
	}

	void FlowControl::leaveEvent(QEvent *event)
	{
		setCursor(Qt::ArrowCursor);
	}

	/*!
	* @brief 更新父窗体位置信息
	* @param[in] offsetPos 待移动位置
	*/
	void FlowControl::updateParentPos(QPoint offsetPos)
	{
		QPoint newPosition = m_parent->pos() + offsetPos;
		QRect newRect(newPosition, m_parent->size());
		QRect screenSize = Base::RUtil::screenGeometry();

		if (!screenSize.contains(newRect)) {
			if (newPosition.x() < screenSize.x())
				newPosition.setX(screenSize.x());
			else if (newPosition.x() > (screenSize.width() + screenSize.x() - newRect.width()))
				newPosition.setX(screenSize.width() + screenSize.x() - newRect.width());

			if (newPosition.y() < screenSize.y())
				newPosition.setY(screenSize.y());
			else if (newPosition.y() > (screenSize.height() + screenSize.y() - newRect.height()))
				newPosition.setY(screenSize.height() + screenSize.y() - newRect.height());
		}

		m_parent->move(newPosition);
	}

} //namespace Base 