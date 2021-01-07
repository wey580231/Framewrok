/*!
 *  @brief     数据概览显示块
 *  @details   以较大的字体、较明亮的颜色显示概要信息，后期可通过点击查看详细内容
 *  @author    wey
 *  @version   1.0
 *  @date      2021.01.07
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#pragma once

#include <QWidget>
#include "ui_overviewitem.h"

namespace Related {

	class OverViewItem : public QWidget
	{
		Q_OBJECT

	public:
		OverViewItem(QWidget *parent = Q_NULLPTR);
		~OverViewItem();

		void setLabelText(QString  text);
		void setLabelData(QString text);
		void setLabelIcon(QString icon);
		void setLabelBackground(QColor color);

	private:
		Ui::OverViewItem ui;
	};

} //namespace Related 