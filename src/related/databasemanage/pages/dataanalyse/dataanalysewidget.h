/*!
 *  @brief     数据分析页面
 *  @details   1.提供录音，以及音频编辑相关操作；
			   2.提供音频分析处理、分析、操作工具等；
 *  @author    wey
 *  @version   1.0
 *  @date      2021.01.07
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#pragma once

#include <QWidget>

#include <base\selfwidget\rtabbar.h>
#include "../abstractpage.h"

namespace Related {

	class DataAnalyseWidget : public AbstractPage
	{
		Q_OBJECT

	public:
		DataAnalyseWidget(QWidget *parent = nullptr);
		~DataAnalyseWidget();

		PageType getPageType() const;

	private:
		void init();
	};

}//namespace Related 