/*!
 * @brief     单个音频文件的态势分析图   
 * @author    wey
 * @version   1.0
 * @date      2021.02.01 16:28:02
 * @warning
 * @copyright NanJing RenGu.
 * @note
 */
 #pragma once

#include "../../abstractpage.h"

namespace Related {

	class RCustomPlot;

	class SituationAnalysis : public AbstractPage
	{
		Q_OBJECT

	public:
		SituationAnalysis(QWidget *parent = nullptr);
		~SituationAnalysis();

		PageType getPageType() const;

	private:
		void init();

	private:
		RCustomPlot * m_mainGraph;
		QWidget * m_mapWidget;
	};

} //namespace Related 