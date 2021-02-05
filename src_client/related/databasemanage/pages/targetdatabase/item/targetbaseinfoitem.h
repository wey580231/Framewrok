/*!
 * @brief     目标的基本信息
 * @details   显示单个目标的
 * @author    yzg
 * @version   1.0
 * @date      2021.02.03 14:22:17
 * @warning
 * @copyright NanJing RenGu.
 * @note
 */
 #pragma once

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QPainter>

#include <base/util/rutil.h>
#include <commondefines/structdefines.h>

#include "../../datastruct.h"

namespace Related {

	class TargetBaseInfoItem : public QWidget
	{
		Q_OBJECT

	public:
		TargetBaseInfoItem(QWidget *parent = nullptr);
		~TargetBaseInfoItem();

	protected:
		void paintEvent(QPaintEvent *event);
		
	private:
		void init();

	};

}//namespace Related 