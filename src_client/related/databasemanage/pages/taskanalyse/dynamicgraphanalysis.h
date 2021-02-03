/*!
 * @brief     动态谱图信息显示区   
 * @author    wey
 * @version   1.0
 * @date      2021.02.01 14:51:35
 * @warning
 * @copyright NanJing RenGu.
 * @note
 */
 #pragma once

#include <related/3rdLibrary/qcustomplot.h>
#include "../../abstractpage.h"

namespace Related {

	class RCustomPlot;
	class TinyWavGraph;

	class DynamicGraphAnalysis : public AbstractPage
	{
		Q_OBJECT

	public:
		DynamicGraphAnalysis(QWidget *parent = nullptr);
		~DynamicGraphAnalysis();

		PageType getPageType() const;

	private:
		void init();
		void initPlot(QCustomPlot * plot);

	private:
		RCustomPlot * m_demoLine;		
		RCustomPlot * m_demoGraph;		
		RCustomPlot * m_lofarLine;		
		RCustomPlot * m_lofarGraph;		

		TinyWavGraph * m_tinyWav;
	};

} //namespace Related