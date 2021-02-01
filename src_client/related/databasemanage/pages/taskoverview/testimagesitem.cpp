#include "testimagesitem.h"

#include <QDebug>
#include <QPainter>
#include <QPixmap>

namespace Related {

	TestImagesItem::TestImagesItem(QWidget *parent)
		: QWidget(parent),
		m_imagesIndex(0),
		m_imagesSuspension(false)
	{

		m_imagesPath = QStringLiteral(":/QYBlue/resource/qyblue/试验记录.png");

		init();
	}

	TestImagesItem::~TestImagesItem()
	{

	}

	void TestImagesItem::setImagesType(TestImagesType type)
	{
		m_imagesType = type;
		if (m_imagesType == TI_Sketch) {
			setCursor(Qt::PointingHandCursor);
		}
	}

	void TestImagesItem::setImagesIndex(int inde)
	{
		m_imagesIndex = inde;
	}

	void TestImagesItem::paintEvent(QPaintEvent * event)
	{
		QPainter painter(this);
		QPixmap pix;
		painter.translate(0, 0);
		pix.load(m_imagesPath);

		if (m_imagesSuspension) {
			painter.drawPixmap(3, 3, width() - 6, height() - 6, pix);
		}
		else
		{
			painter.drawPixmap(6, 6, width() - 12, height() - 12, pix);
		}


		// 描述信息
		if (m_imagesType == TI_Sketch) {
			painter.setRenderHint(QPainter::Antialiasing, true);
			painter.setPen(QColor(0, 255, 255));
			if (m_imagesSuspension) {
				painter.drawRect(3, 3, width() - 6, height() - 6);
			}
			else
			{
				painter.drawRect(6, 6, width() - 12, height() - 12);
			}

			painter.setPen(QColor(255, 255, 255));
			painter.drawText(10, height() - 16, QStringLiteral("20210120 试验记录"));
		}
		QWidget::paintEvent(event);
	}

	void TestImagesItem::mousePressEvent(QMouseEvent * event)
	{
		if (event->button() == Qt::LeftButton) {
			if (m_imagesType == TI_Sketch) {
				emit signalSeleteImagesIndex(m_imagesIndex);
			}
		}
	}

	void TestImagesItem::enterEvent(QEvent * event)
	{
		if (m_imagesType == TI_Sketch) {
			m_imagesSuspension = true;
			this->update();
		}
	}

	void TestImagesItem::leaveEvent(QEvent * event)
	{
		if (m_imagesType == TI_Sketch) {
			m_imagesSuspension = false;
			this->update();
		}
	}

	void TestImagesItem::init()
	{

	}

}//namespace Related 