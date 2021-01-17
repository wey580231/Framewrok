﻿/*!
 *  @brief     自定义按钮
 *  @details
 *  @file      rbutton.h
 *  @author    wey
 *  @version   1.0
 *  @date      2017.12.21
 *  @warning
 *  @copyright NanJing RenGu.
 */
#ifndef RBUTTON_H
#define RBUTTON_H

#include <QPushButton>
#include "../base_global.h"

namespace Base {

	class RButtonPrivate;

	class BASESHARED_EXPORT RButton : public QPushButton
	{
		Q_OBJECT
			Q_DECLARE_PRIVATE(RButton)
	public:
		RButton(QWidget * parent = 0);

		void setText(const QString &text);

	private:
		RButtonPrivate * d_ptr;

	};
} //namespace Base 

#endif // RBUTTON_H
