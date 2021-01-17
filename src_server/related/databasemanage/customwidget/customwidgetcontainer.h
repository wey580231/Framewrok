/*!
 * @brief     通用窗体外框，可统一设定窗体的样式   
 * @author    wey
 * @version   1.0
 * @date      2021.01.17 00:20:21
 * @warning
 * @copyright NanJing RenGu.
 * @note
 */
 #pragma once

#include <QWidget>

namespace Related {

	class CustomWidgetContainer : public QWidget
	{
		Q_OBJECT

	public:
		CustomWidgetContainer(QWidget *parent = nullptr);
		~CustomWidgetContainer();

		void setContent(QWidget * widget);

	protected:
		void paintEvent(QPaintEvent *event);

	private:
		void init();

	};

} //namespace Related 