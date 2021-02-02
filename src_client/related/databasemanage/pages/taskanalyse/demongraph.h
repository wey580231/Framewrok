/*!
 * @brief     DEMONÆ×Í¼   
 * @author    wey
 * @version   1.0
 * @date      2021.02.01 10:48:33
 * @warning
 * @copyright NanJing RenGu.
 * @note
 */
 #pragma once

#include "../../abstractpage.h"

namespace Related {

	class RCustomPlot;
	class TinyWavGraph;

	class DemonGraph : public AbstractPage
	{
		Q_OBJECT

	public:
		DemonGraph(QWidget *parent = nullptr);
		~DemonGraph();

		PageType getPageType() const;

	private:
		void init();

	private:
		RCustomPlot * m_mainGraph;
		TinyWavGraph * m_tinyWav;
	};

} //namespace Related 