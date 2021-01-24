/*!
 * @brief     任务分析页面  
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


#include <base\selfwidget\rtabbar.h>
#include "../abstractpage.h"

#include "dataoverviewwidget.h"
#include "audiodatawidget.h"
#include "ambientnoisedatawidget.h"
#include "hxjplatforminfowidget.h"

namespace Related {

	class TaskAnalysePage : public AbstractPage
	{
		Q_OBJECT

	public:
		TaskAnalysePage(QWidget *parent = nullptr);
		~TaskAnalysePage();

		PageType getPageType() const;

		enum TaskAnalyseIndex
		{
			Tab_dataOverview,			/*!<  数据概览界面 */
			Tab_advInfo,				/*!< 音频信息界面 */
			Tab_ambientNoise,			/*!< 环境噪声界面 */
			Tab_hxjInfo,				/*!< HXJ平台界面 */
		};

	private slots:
		void respTabChanged(int page);

	private:
		void init();

	private:
		Base::RTabBar * m_tabWidget;
		QStackedWidget * m_stackedWidget;

		DataOverviewWidget * m_dataOverviewWidget;			/*!< 数据雨啦界面 */
		AudioDataWidget * m_audioDataWidget;				/*!< 音频数据展示界面 */
		AmbientNoiseDataWidget * m_ambientNoiseDataWidget;	/*!< 环境噪声数据展示界面 */
		HXJPlatformInfoWidget * m_hxjPlatformDataWidget;	/*!< HXJ平台信息展示界面 */
	};

} //namespace Related 