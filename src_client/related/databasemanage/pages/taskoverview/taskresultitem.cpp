#include "taskresultitem.h"

#include <QDebug>
#include <QPainter>
#include <QPixmap>

namespace Related {

	TaskResultItem::TaskResultItem(QWidget *parent)
		: QWidget(parent),
		m_pictureIndex(0), m_selectedstatus(false)
	{
		
	}

	TaskResultItem::~TaskResultItem()
	{

	}

	void TaskResultItem::setPictureIndex(int index)
	{
		this->setWindowTitle(QString("%1_%2").arg(QStringLiteral("Í¼Æ¬£º")).arg(QString::number(index)));
			m_pictureIndex = index;
	}

	int TaskResultItem::getPictureIndex()
	{
		return m_pictureIndex;
	}

	void TaskResultItem::setPicturePath(QString path)
	{
		m_picturePath = path;
	}

	QString TaskResultItem::getPicturePath()
	{
		return m_picturePath;
	}

	void TaskResultItem::setSelectedstatus(bool status)
	{
		m_selectedstatus = status;
	}

	void TaskResultItem::updateItem()
	{
		this->update();
	}

	void TaskResultItem::paintEvent(QPaintEvent * event)
	{

		//[] »æÖÆÍ¼Æ¬
		QPainter painter(this);
		QPixmap pix;
		painter.translate(0, 0);
		pix.load(m_picturePath);
		painter.drawPixmap(5, 5, width()-10, height()-10, pix);//»æÖÆÍ¼Æ¬ ºá×ø±ê¡¢×Ý×ø±ê¡¢¿í¶È¡¢¸ß¶È

		//[] »æÖÆÑ¡Ôñ±ß¿ò
		if (m_selectedstatus == true) {
			// ·´×ßÑù
			painter.setRenderHint(QPainter::Antialiasing, true);
			// ÉèÖÃ»­±ÊÑÕÉ«
			painter.setPen(QColor(255, 0, 51));

			painter.drawRect(3, 3, width()-6, height()-6);
		}

		QWidget::paintEvent(event);
	}

	void TaskResultItem::mousePressEvent(QMouseEvent * event)
	{
		// Êó±ê×ó¼ü
		if (event->button() == Qt::LeftButton){
			m_selectedstatus = true;
			this->update();
			emit signalPictureIndex(m_pictureIndex, m_picturePath);
		}
	}

	void TaskResultItem::init()
	{
	}

}//namespace Related 