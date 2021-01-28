/*!
 * @brief     无边框MainWindow
 * @details	  自定义无边框包括窗口标题栏和中心显示窗口两部分，
			  标题栏:从左至右依次为：程序图标、自定义菜单栏区、最小按钮、最大按钮、关闭按钮
			  这些按钮控件均可控制显隐。
 *			  |[Icon][CustomMenuWidget]----[Min][Max][Close]|
 *            |											    |
 *            |                                     	    |
 *            |                                             |
 *            |               CenterWidget                  |
 *            |                                             |
 *            |                                             |
 *            |                                             |
 *            -----------------------------------------------
 * @author    wey
 * @version   1.0
 * @date      2021.01.28 08:55:11
 * @warning
 * @copyright NanJing RenGu.
 * @note
 */
#pragma once

#include <QMainWindow>
class QPushButton;

#include "../macros.h"
#include "../base_global.h"
#include "iconbutton.h"

namespace Base {

	class BASESHARED_EXPORT RBorderLessMainWindow : public QMainWindow
	{
		Q_OBJECT

	public:
		/*! 
		 * @brief 设置标题栏是否与CentralWidget处于同一布局内
		 * @param titleBarHoverWindow true:标题栏区域悬浮在centralWidget的上部
		 * @return 
		 */
		explicit RBorderLessMainWindow(bool titleBarHoverWindow = false,QWidget * parent = nullptr);

		enum ButtonFlag {
			IconButt = 0x0001,
			MenuWidget = 0x0002,
			MinButt = 0x0004,
			MaxButt = 0x0008,
			CloseButt = 0x0010,

			AllButt = IconButt | MenuWidget | MinButt | MaxButt | CloseButt
		};
		Q_DECLARE_FLAGS(ButtonFlags, ButtonFlag)
		Q_DECLARE_FRIEND_FLAGS(ButtonFlags)

		void setButtonEnable(ButtonFlags bf,bool enabled = true);

		/*! 
		 * @brief 设置窗口左上角程序图标
		 * @param iconPath 图标路径
		 */
		void setProgramIcon(const QString &iconPath);

		void setCentralWidget(QWidget * widget);

		/*!
		 * @brief 在标题栏中设置自定义菜单栏
		 */
		void setCustomMenuWidget(QWidget * widget);

		/*!
		 * @brief 设置窗口阴影是否可见
		 * @param visible true:可见，false:隐藏
		 */
		void setBorderShadowVisible(bool visible = true);

	protected:
		bool nativeEvent(const QByteArray &eventType, void *message, long *result) override;
		bool eventFilter(QObject *o, QEvent *e);
		void mousePressEvent(QMouseEvent *event) override;
		void changeEvent(QEvent* e) override;

	private slots:
		void slot_show_system_menu() const;
		void slot_minimized();
		void slot_maximized();
		void slot_restored();
		void slot_closed();

	private:
		void init();
		void updateButtState();

		auto set_borderless(bool enabled) const -> void;

	private:
		ButtonFlags m_buttFlags;

		QWidget * m_titleBarWidget;
		RIconButton * m_minButton;
		RIconButton * m_maxButton;
		RIconButton * m_restoreButton;
		RIconButton * m_closeButton;
		QPushButton * m_iconButton;

		QWidget * m_customMenuWidget;
		QWidget * m_centralWidget;

		bool m_titleBarHoverWindow;
	};

}//namespace Base 
