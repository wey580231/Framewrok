#pragma once

#include <QWidget>

#include <related/3rdLibrary/qcustomplot.h>

namespace Related {

	class RCustomPlot : public QCustomPlot
	{
		Q_OBJECT

	public:
		RCustomPlot(QWidget *parent = nullptr);
		~RCustomPlot();

	private:
		void init();
	};

} //namespace Related 