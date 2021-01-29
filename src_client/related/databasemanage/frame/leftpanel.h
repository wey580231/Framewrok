/*!
 *  @brief     左侧操作窗口
 *  @details   包含标题区、列表操作区、用户管理等子区域
 *  @author    wey
 *  @version   1.0
 *  @date      2021.01.07
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#pragma once

#include <QWidget>
#include <QLabel>
#include <QStackedWidget>

#include <base\selfwidget\iconbutton.h>
#include <base\util\widgetanimation.h>

#include "../datastruct.h"

namespace Related {

	class RListWidget;

	class LeftPanel : public QWidget
	{
		Q_OBJECT

	public:
		LeftPanel(QWidget *parent);
		~LeftPanel();

		/*!
		 * @brief 切换左侧菜单至指定模式
		 * @note 默认显示第一个菜单项
		 * @param model 待切换的显示模式
		 */
		void switchViewModel(ViewModel model);

		int getCurrentPage() const;

	signals:
		void currentIndexChanged(int);
		void switchToSystemView();
		void reConnectToServer();

	private slots:
		void respNetStateChanged(bool isConnectToServer);
		void respLeftPanelExpand(bool checked);
		void backToSystemView();

	private:
		void init();

	private:
		RListWidget * m_systemListWidget;	/*!< 系统菜单列表 */	
		RListWidget * m_taskListWidget;		/*!< 任务菜单列表 */
		QStackedWidget * m_leftMenuContainer;

		ViewModel m_currViewModel;			/*!< 当前视图模式 */
		
		Base::WidgetAnimation m_animation;

		QLabel * m_prgoramIcon;
		Base::RIconButton * m_expandButt;
		int m_expandStateWidth;		//展开状态下宽度
		int m_contractionWidth;		//收缩状态下宽度

		Base::RIconButton * m_notifyButt;		/*!< 通知 */
		Base::RIconButton * m_netStateButt;		/*!< 网络状态通知 */
		Base::RIconButton * m_backToSystemViewButt;		/*!< 返回系统视图 */
	};

} //namespace Related 
