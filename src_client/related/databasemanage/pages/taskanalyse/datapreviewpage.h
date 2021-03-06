/*!
 * @brief      数据预览界面
 * @details
 * @author    yzg
 * @version   1.0
 * @date      2021.01.23 14:18:49
 * @warning
 * @copyright NanJing RenGu.
 * @note
 */
#pragma once

#include <QWidget>
#include <QLabel>
#include <QDateTimeEdit>
#include <QComboBox>
#include <QHBoxLayout>
#include <QMenu>
#include <QAction>

#include <base\selfwidget\tablemode\rtableview.h>
#include <base\selfwidget\iconbutton.h>

#include "../abstractpage.h"
#include "../../customwidget/operationtoolspage.h"
#include "../../customwidget/pageswitchbar.h"
#include "tablemodel/dataoverviewmolel.h"

namespace Related {

	class DataPreviewPage : public AbstractPage
	{
		Q_OBJECT

	public:
		DataPreviewPage(QWidget *parent = nullptr);
		~DataPreviewPage();

		PageType getPageType() const;

		void prepareBringToTop();

		void setTaskId(QString taskId);

	signals:
		void signalOpenAudioDataPage(QString dataId);

	private slots:
		void respToolButtPressed(OperationToolsPage::ButtType type);
		void slotTableDoubleClicked(QModelIndex index);
		void setPageNum(int page);
		void setFixedPageRowCount(int pageItemCount);

		void showTableContextMenu(const QPoint & point);
		void analyseWav(bool flag);
		void viewWavOriginalData(bool flag);

	private:
		void init();
		void initConnect();

	private:
		OperationToolsPage *m_operationToolsPage;
		Base::RTableView * m_tableView;
		DataOverviewMolel *m_tableModel;
		PageSwitchBar * m_pageSwitch;

		bool m_firstLoadData;									/*!< 第一次加载页面显示 */
		QString m_taskId;										/*!< 任务Id */
		int m_seleteTableRow;									/*!< 选中行数 */

		QMenu * m_tableMenu;
		QAction * m_analyseWav;				/*!< 分析音频 */
		QAction * m_viewOriginalData;		/*!< 查看原始数据 */
	};

}//namespace Related 