/*!
 * @brief     自定义Tab窗口
 * @details	  基于RTabBar + QStackedWidget实现
 * @author    wey
 * @version   1.0
 * @date      2021.01.26 10:37:27
 * @warning
 * @copyright NanJing RenGu.
 * @note
 */
 #pragma once

#include <functional>

#include <QWidget>
#include <QStackedWidget>

#include "../base_global.h"
#include "rtabbar.h"

namespace Base {

	class BASESHARED_EXPORT RTabWidget : public QWidget
	{
		Q_OBJECT

	public:
		RTabWidget(QWidget *parent = nullptr);
		~RTabWidget();

		void addPage(QString tabName,QWidget * widget);
		int pageCount() const;

		QWidget * currentWidget();

		void setTabBarHeight(int fixedHeight);
		int tabBarHeight() const;

		void setSpacing(int spacing);
		void setContentMargins(int left,int top,int right,int bottom);

		void setTabAligment(TabAlignment alignment);

		typedef std::function<void(QWidget *)> WidgetCallback;

		/*! 
		 * @brief 设置StackedWidget中当前窗口被切换至底层时回调接口
		 * @details 可用于关闭当前页面中未完成的任务，如清空页面显示等
		 * @param callback 回调函数
		 */
		void setWidgetBringToBottomCallback(WidgetCallback callback);

		/*!
		 * @brief 设置StackedWidget中新窗口被切换至顶部显示时回调接口
		 * @details 可用于新窗口的初始化，如第一次发送请求等
		 * @param callback 回调函数
		 */
		void setWidgetBringToTopCallback(WidgetCallback callback);

	private slots:
		void prepareSwitchWidget(int index);

	private:
		void init();

	private:
		RTabBar * m_tabBar;
		QStackedWidget * m_widgetContainer;

		WidgetCallback m_bingToBottomCallback;		
		WidgetCallback m_bringToTopCallback;	
	};

} //namespace Base 