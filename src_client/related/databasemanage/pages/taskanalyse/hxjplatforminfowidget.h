/*!
 * @brief	  HXJ平台数据展示界面
 * @details 
 * @author    yzg
 * @version   1.0
 * @date      2021.01.23 14:48:21
 * @warning
 * @copyright NanJing RenGu.
 * @note
 */ 
#pragma once

#include <QWidget>
#include <QComboBox>

#include <base\selfwidget\tablemode\rtableview.h>
#include <base\selfwidget\iconbutton.h>

#include "customwidget/operationtoolspage.h"
#include "tablemodel/hxjplatforminfomodel.h"

namespace Related {

	class HXJPlatformInfoWidget : public QWidget
	{
		Q_OBJECT

	public:
		HXJPlatformInfoWidget(QWidget *parent);
		~HXJPlatformInfoWidget();

	private:
		void init();

	private:
		Base::RTableView * m_tableView;
		HXJPlatformInfoModel *m_tableModel;
		OperationToolsPage *m_operationToolsPage;
	};

}// namespace Related 