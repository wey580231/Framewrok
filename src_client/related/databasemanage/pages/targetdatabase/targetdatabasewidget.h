/*!
 * @brief     目标库界面  
 * @details   提供
 * @author    yzg
 * @version   1.0
 * @date      2021.01.20 10:43:37
 * @warning
 * @copyright NanJing RenGu.
 * @note
 */
#pragma once

#include <QWidget>
#include <qstackedwidget.h>

#include <base\selfwidget\rtabwidget.h>

#include "../abstractpage.h"

namespace Related {

	class TargetOverViewPage;
	class TargetDetailPage;

	class FrequencyDomainAnalyse;
	class TimeDomainAnalyse;
	class DynamicGraphAnalysis;
	class DemonGraph;
	class LofarGraph;
	class SituationAnalysis;

	class TargetDatabaseWidget : public AbstractPage
	{
		Q_OBJECT

	public:
		TargetDatabaseWidget(QWidget *parent = nullptr);
		~TargetDatabaseWidget();

		PageType getPageType() const;

		void prepareBringToTop();

	private slots:
		void slotSelecteTargetDataInfo(QString indexId);

	private:
		void init();

	private:
		Base::RTabWidget * m_tabWidget;							/*!<  */
		TargetOverViewPage * m_targetOverViewPage;				/*!< 目标库概览界面 */
		TargetDetailPage * m_targetDetailPage;					/*!< 目标详情页面 */

		TimeDomainAnalyse * m_timeAnalyseWidget;			/*!< 时域分析页面 */
		DynamicGraphAnalysis * m_audioDataWidget;			/*!< 频分析界面 */
		DemonGraph * m_demonAnalysis;						/*!< demon分析界面 */
		LofarGraph * m_lofarAnalysis;						/*!< lfoar分析界面 */
		SituationAnalysis * m_situationAnalysis;			/*!< 态势分析页面 */

	};

}//namespace Related 