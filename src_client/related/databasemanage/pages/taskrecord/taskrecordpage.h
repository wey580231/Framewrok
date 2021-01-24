/*!
 * @brief     任务记录页面
 * @details   提供值更记录、试验记录等页面
 * @author    wey
 * @version   1.0
 * @date      2021.01.20 09:16:07
 * @warning
 * @copyright NanJing RenGu.
 * @note
 */
 #pragma once

#include <QWidget>
#include <QStackedWidget>

#include <base\selfwidget\rtabbar.h>
#include "../abstractpage.h"

#include "logbookpage.h"
#include "trialsheetwidget.h"
#include "taskreportswidget.h"

namespace Related {

	class TaskRecordPage : public AbstractPage
	{
		Q_OBJECT

	public:
		TaskRecordPage(QWidget *parent = nullptr);
		~TaskRecordPage();

		PageType getPageType() const;

		enum TaskRecordIndex {
			Tab_logbook,			/*!< 值班日志 */
			Tab_trialSheet,			/*!< 试验记录 */
			Tab_taskReports,		/*!< 任务报告 */
		};


	private slots:
		void  slotRespTabChanged(int page);

	private:
		void init();

	private:
		Base::RTabBar * m_tabWidget;
		QStackedWidget * m_stackedWidget;

		LogbookPage *m_logbookPage;						/*!< 值班日志界面 */
		TrialSheetWidget * m_trialSheetWidget;			/*!< 试验记录窗口 */
		TaskReportsWidget * m_taskReportsWidget;		/*!< 任务报告界面 */
	}; 

} //namespace Related 