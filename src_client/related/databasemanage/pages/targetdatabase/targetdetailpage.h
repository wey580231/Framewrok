/*!
 * @brief     单个目标详情页面   
 * @details
 * @author    wey
 * @version   1.0
 * @date      2021.02.03 10:42:17
 * @warning
 * @copyright NanJing RenGu.
 * @note
 */
 #pragma once

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>

#include <base\selfwidget\tablemode\rtableview.h>
#include <base\selfwidget\iconbutton.h>
#include <base/selfwidget/rmessagebox.h>
#include <base\util\rutil.h>
#include <commondefines/protocol.h>

#include "../abstractpage.h"
#include "../../customwidget/pageswitchbar.h"
#include "../../customwidget/operationtoolspage.h"

#include "item/targetbaseinfoitem.h"
#include "tablemodel/targetwavdatamodel.h"

namespace Related {

	class TargetDetailPage : public AbstractPage
	{
		Q_OBJECT

	public:
		TargetDetailPage(QWidget *parent);
		~TargetDetailPage();

		PageType getPageType() const;

		void prepareBringToTop();

	private slots:

	private:
		void init();
		void initConnect();

	private:
		TargetBaseInfoItem * m_targetBaseInfoItem;				/*!< 目标基本信息  */

		OperationToolsPage * m_operationToolsPage;				/*!< 操作工具页面 */
		Base::RTableView * m_tableView;							/*!< 数据表格 */
		TargetWavDataModel * m_tableModel;						/*!< 数据模型 */
		PageSwitchBar * m_pageSwitch;

		bool m_firstLoadData;									/*!< 第一次加载页面显示 */
	};

} //namespace Related 