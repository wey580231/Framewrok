#include "rcustomplot.h"

namespace Related {

	RCustomPlot::RCustomPlot(QWidget *parent)
		: QCustomPlot(parent)
	{
		init();
	}

	RCustomPlot::~RCustomPlot()
	{
	}

	void RCustomPlot::init()
	{
		auto setAxisStyle = [](QCPAxis * axis) {
			QColor baseColor(118, 121, 124);
			axis->setBasePen(QPen(baseColor, 1.5));
			axis->setTickPen(QPen(baseColor, 1.5));
			axis->setSubTickPen(baseColor);

			QColor tickColor(143, 222, 231);
			QFont labelFont(QStringLiteral("Î¢ÈíÑÅºÚ"), 11);
			axis->setTickLabelFont(labelFont);
			axis->setTickLabelColor(tickColor);

			labelFont.setPixelSize(13);
			axis->setLabelFont(labelFont);
			axis->setLabelColor(tickColor);
		};

		this->yAxis->setVisible(true);
		this->yAxis->setTickLabels(true);
		this->yAxis->setLabel(QStringLiteral("·ùÖµ"));

		this->yAxis2->setVisible(false);
		this->yAxis2->setTickLabels(false);

		this->xAxis->setVisible(true);
		this->xAxis->setTickLabels(true);
		this->xAxis->setLabel(QStringLiteral("Hz"));

		this->xAxis2->setVisible(false);
		this->xAxis2->setTickLabels(true);

		this->setBackground(QColor(0, 0, 0, 0));
		setAxisStyle(xAxis);
		setAxisStyle(yAxis);
		setAxisStyle(xAxis2);
		setAxisStyle(yAxis2);
	}

} //namespace Related 