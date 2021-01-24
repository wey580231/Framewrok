#pragma once

#include <QWidget>

#include "../datastruct.h"

namespace Related {

	class AbstractPage : public QWidget
	{
		Q_OBJECT

	public:
		AbstractPage(QWidget *parent);
		~AbstractPage();

		virtual PageType getPageType() const = 0;

		/*!
		 * @brief 页面将被切换至最上层显示
		 * @details 可以用来做页面第一次初始化等回调
		 */
		virtual void prepareBringToTop() {}

		/*!
		 * @brief 页面将被切换至非显示状态
		 * @details 可以用来关闭未处理完的操作
		 */
		virtual void prepareBringToBottom() {}

	};

} //namespace Related 