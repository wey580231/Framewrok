#include "tinywavgraph.h"

#include <QHBoxLayout>
#include <QEvent>
#include <QPainter>

namespace Related {

	TinyWavGraph::TinyWavGraph(QWidget *parent)
		: QWidget(parent)
	{
		init();
	}

	TinyWavGraph::~TinyWavGraph()
	{
	}

	bool TinyWavGraph::eventFilter(QObject * watched, QEvent * event)
	{
		if (watched == m_waveGraph) {
			if (event->type() == QEvent::Paint) {
				QPainter painter(m_waveGraph);
				painter.setPen(QColor(107, 244, 183));

				painter.save();
				painter.setPen(Qt::NoPen);
				painter.setBrush(QColor(144, 144, 144, 100));
				painter.drawRoundedRect(m_waveGraph->rect(), 3, 3 );
				painter.restore();

				int hcount = m_waveGraph->width();
				int vcount = m_waveGraph->height();
				painter.translate(0, m_waveGraph->height() / 2);
				for (int i = 0; i < hcount; i++) {
					int rheight = qrand() % (vcount / 2);
					painter.drawLine(QPoint(i, -rheight), QPoint(i, rheight));
				}
			}
		}
		return QWidget::eventFilter(watched, event);
	}

	void TinyWavGraph::init()
	{
		m_waveGraph = new QWidget();
		m_waveGraph->installEventFilter(this);

		QHBoxLayout * layout = new QHBoxLayout();
		layout->setContentsMargins(0, 0, 0, 0);
		layout->addWidget(m_waveGraph);
		this->setLayout(layout);
	}

} //namespace Related 