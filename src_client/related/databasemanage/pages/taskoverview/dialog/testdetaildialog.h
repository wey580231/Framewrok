/*!
 * @brief     试验详图显示对话框  
 * @details 
 * @author    yzg
 * @version   1.0
 * @date      2021.02.01 09:58:43
 * @warning
 * @copyright NanJing RenGu.
 * @note
 */
#pragma once

#include <QWidget>

#include <base/selfwidget/dialogproxy.h>
#include <base\selfwidget\tablemode\rtableview.h>
#include <base/selfwidget/iconbutton.h>
#include <base/selfwidget/rtabwidget.h>
#include <base\selfwidget\rmessagebox.h>
#include <base/util/rutil.h>

namespace Related {

	class TestDetailDialog : public Base::DialogProxy
	{
		Q_OBJECT

	public:
		TestDetailDialog(QWidget *parent = nullptr);
		~TestDetailDialog();

	private:
		void init();
	};

}//namespace Related 