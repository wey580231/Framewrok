/*!
 * @brief     音频数据信息展示界面  
 * @details
 * @author    yzg
 * @version   1.0
 * @date      2021.01.23 14:24:08
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
#include "tablemodel/audiodatamodel.h"

namespace Related {

	class AudioDataWidget : public QWidget
	{
		Q_OBJECT

	public:
		AudioDataWidget(QWidget *parent);
		~AudioDataWidget();


	private:
		void init();

	private:
		Base::RTableView * m_tableView;
		AudioDataModel *m_tableModel;
		OperationToolsPage *m_operationToolsPage;
	};

}//namespace Related 