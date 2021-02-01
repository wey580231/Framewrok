/*!
 * @brief     任务分析页面
 * @details
 * @author    wey
 * @version   1.0
 * @date      2021.01.20 09:18:29
 * @warning
 * @copyright NanJing RenGu.
 * @note
 */
#pragma once

#include <QWidget>
#include <QStackedWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include <base\selfwidget\rtabwidget.h>
#include "../abstractpage.h"

#include "datapreviewpage.h"
#include "ambientnoisedatawidget.h"
#include "hxjplatforminfowidget.h"

namespace Related {

	class FrequencyDomainAnalyse;
	class TimeDomainAnalyse;
	class DynamicGraphAnalysis;
	class DemonGraph;
	class LofarGraph;
	class SituationAnalysis;

	class TaskAnalysePage : public AbstractPage
	{
		Q_OBJECT

	public:
		TaskAnalysePage(QWidget *parent = nullptr);
		~TaskAnalysePage();

		PageType getPageType() const;

		void prepareBringToTop();

		void setTaskId(QString taskId);

	private slots:
		void slotOpenAudioDataPage(QString dataId);

	private:
		void init();

	private:
		Base::RTabWidget * m_tabWidget;

		DataPreviewPage * m_dataPreviewPage;				/*!< 数据预览界面 */
		TimeDomainAnalyse * m_timeAnalyseWidget;			/*!< 时域分析页面 */
		DynamicGraphAnalysis * m_audioDataWidget;			/*!< 频分析界面 */
		DemonGraph * m_demonAnalysis;						/*!< demon分析界面 */
		LofarGraph * m_lofarAnalysis;						/*!< lfoar分析界面 */
		SituationAnalysis * m_situationAnalysis;			/*!< 态势分析页面 */

		AmbientNoiseDataWidget * m_ambientNoiseDataWidget;	/*!< 环境噪声数据展示界面 */
		HXJPlatformInfoWidget * m_hxjPlatformDataWidget;	/*!< HXJ平台信息展示界面 */
	};

} //namespace Related 