/*!
 *  @brief     位置移动控制工具栏
 *  @details   用于地图工具栏中，用于控制主窗体移动;
 *             若bindMoveWidget()指定了窗体，则会以此窗体为目标更新位置信息，否则将发出posDiff信号
 *  @author    wey
 *  @version   1.0
 *  @date      2019.06.20
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef FLOWCONTROL_H
#define FLOWCONTROL_H

#include <QWidget>
#include <QSharedPointer>
#include <QMenu>

#include "../base_global.h"

namespace Base {

	class BASESHARED_EXPORT FlowControl : public QWidget
	{
		Q_OBJECT
	public:
		explicit FlowControl(QWidget *  parent = 0);
		~FlowControl() {}

		void addAction(QAction * action);

		void bindMoveWidget(QWidget * widget) { m_parent = widget; }

	signals:
		void posDiff(QPoint offsetPos);

	protected:
		void mousePressEvent(QMouseEvent * event);
		void mouseMoveEvent(QMouseEvent * event);
		void mouseReleaseEvent(QMouseEvent * event);
		void paintEvent(QPaintEvent * event);
		void contextMenuEvent(QContextMenuEvent * event);
		void enterEvent(QEvent *event);
		void leaveEvent(QEvent * event);

	private:
		void updateParentPos(QPoint offsetPos);

	private:
		QPoint m_globalMouseStartPoint;       /*!< 全局坐标系下鼠标按下的位置 */
		bool m_b_leftButtonPressed;
		QWidget * m_parent;     /*!< 父窗体 */

		QMenu * m_menu;
	};

} //namespace Base 

#endif // FLOWCONTROL_H
