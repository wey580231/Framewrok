#include "customwidgetcontainer.h"

#include <QHBoxLayout>
#include <QRadialGradient>
#include <QPainter>

namespace Related {

	CustomWidgetContainer::CustomWidgetContainer(QWidget *parent)
		: QWidget(parent)
	{
		setObjectName("CustomWidgetContainer");
		init();
	}

	CustomWidgetContainer::~CustomWidgetContainer()
	{
	}

	void CustomWidgetContainer::setContent(QWidget * widget)
	{
		if (layout() == nullptr) {
			QHBoxLayout * hlayout = new QHBoxLayout();
			hlayout->setContentsMargins(4, 4, 4, 4);

			hlayout->addWidget(widget);
			setLayout(hlayout);
		}
		else {
			QHBoxLayout * hlayout = dynamic_cast<QHBoxLayout *>(layout());
			for (int i = hlayout->count(); i >= 0; i--) {
				hlayout->removeItem(hlayout->itemAt(i));
			}
			hlayout->addWidget(widget);
		}
	}

	void CustomWidgetContainer::paintEvent(QPaintEvent *event)
	{
		QPainter painter(this);

		QRect innerRect = rect();
		QPoint center = innerRect.center();
		innerRect.adjust(1, 1, -1, -1);

		painter.setPen(Qt::NoPen);

		QRadialGradient gradient(center.x(), center.y(), innerRect.width() / 2, center.x(), center.y());
		gradient.setColorAt(0, QColor(16, 65, 179, 40));
		gradient.setColorAt(1.0, QColor(50, 101, 184, 50));
		painter.setBrush(QBrush(gradient));
		//painter.setBrush(QColor(16, 65, 179, 210));
		painter.drawRect(innerRect);

		painter.setPen(QPen(QColor(71, 175, 255), 1.1));

		int radius = 8;

		//左上
		painter.drawLine(innerRect.topLeft(), QPoint(1, radius));
		painter.drawLine(innerRect.topLeft(), QPoint(radius, 1));

		//右上
		painter.drawLine(QPoint(innerRect.right() - radius, 1), innerRect.topRight());
		painter.drawLine(innerRect.topRight(), QPoint(innerRect.right(), radius));

		//左下
		painter.drawLine(QPoint(1, innerRect.bottom() - radius), innerRect.bottomLeft());
		painter.drawLine(innerRect.bottomLeft(), QPoint(radius, innerRect.bottom()));

		//右下
		painter.drawLine(QPoint(innerRect.right() - radius, innerRect.bottom()), innerRect.bottomRight());
		painter.drawLine(innerRect.bottomRight(), QPoint(innerRect.right(), innerRect.bottom() - radius));

		QWidget::paintEvent(event);
	}

	void CustomWidgetContainer::init()
	{

	}

} //namespace Related 