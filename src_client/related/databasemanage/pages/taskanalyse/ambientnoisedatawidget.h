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
#include <QHBoxLayout>

#include <base\selfwidget\tablemode\rtableview.h>
#include <base\selfwidget\iconbutton.h>

#include "../abstractpage.h"
#include "../../customwidget/operationtoolspage.h"
#include "tablemodel/ambientnoisemodel.h"
#include "../../customwidget/pageswitchbar.h"

namespace Related {

	class AmbientNoiseDataWidget : public AbstractPage
	{
		Q_OBJECT

	public:
		AmbientNoiseDataWidget(QWidget *parent);
		~AmbientNoiseDataWidget();

		PageType getPageType() const;

	private:
		void init();

	private:

	};

}//namespace Related
