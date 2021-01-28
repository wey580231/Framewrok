/*!
 * @brief    环境噪声信息显示界面
 * @details
 * @author    yzg
 * @version   1.0
 * @date      2021.01.23 14:31:02
 * @warning
 * @copyright NanJing RenGu.
 * @note
 */
#pragma once

#include <QWidget>
#include <QComboBox>

#include <base\selfwidget\tablemode\rtableview.h>
#include <base\selfwidget\iconbutton.h>

#include "../../customwidget/operationtoolspage.h"
#include "tablemodel/ambientnoisemodel.h"

namespace Related {

	class AmbientNoiseDataWidget : public QWidget
	{
		Q_OBJECT

	public:
		AmbientNoiseDataWidget(QWidget *parent);
		~AmbientNoiseDataWidget();

	private:
		void init();

	private:
		Base::RTableView * m_tableView;
		AmbientNoiseModel *m_tableModel;
		OperationToolsPage *m_operationToolsPage;
	};

}//namespace Related
