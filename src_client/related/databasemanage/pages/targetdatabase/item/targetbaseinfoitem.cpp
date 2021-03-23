#include "targetbaseinfoitem.h"

namespace Related {

	TargetBaseInfoItem::TargetBaseInfoItem(QWidget *parent)
		: QWidget(parent)
	{
		this->setMinimumHeight(150);
		this->setMaximumHeight(200);
		init();
	}

	TargetBaseInfoItem::~TargetBaseInfoItem()
	{
	}

	void TargetBaseInfoItem::paintEvent(QPaintEvent * event)
	{
		int t_i_wide = this->width();
		int t_i_high = this->height();
		// 计算等分宽度比
		int  t_temp = t_i_wide / 4;
		// 绘制窗口分割线
		QPainter painter(this);
		painter.setRenderHint(QPainter::Antialiasing);
		painter.setRenderHint(QPainter::SmoothPixmapTransform);
		painter.save();
		for (int i = 1; i < 4; i++) {
			QLineF line(t_temp * i, 0 + 20, t_temp * i, t_i_high - 20);
			painter.drawLine(line);
		}
		painter.restore();
		QWidget::paintEvent(event);
	}

	void TargetBaseInfoItem::init()
	{
	}

}//namespace Related 