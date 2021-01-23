/*!
 *  @brief     扩展QWidget功能
 *  @details   1.可直接将子窗口作为内容,调用setContentWidget,可自动添加布局
 *  @author
 *  @version   1.0
 *  @date      2020.08.05
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef RWIDGET_H
#define RWIDGET_H

#include <QWidget>

#include "../base_global.h"

namespace Base {

	class BASESHARED_EXPORT RWidget : public QWidget
	{
		Q_OBJECT
	public:
		RWidget(QWidget * parent = nullptr);
		~RWidget();

		void setContentWidget(QWidget * childWidget);

	protected:
		QWidget * m_mainWidget;

	};

} //namespace Base

#endif // RWIDGET_H
