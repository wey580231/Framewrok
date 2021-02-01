/*!
 * @brief     试验图片详情显示对话框  
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
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QLabel>

#include <base/selfwidget/dialogproxy.h>
#include <base\selfwidget\tablemode\rtableview.h>
#include <base/selfwidget/iconbutton.h>
#include <base/selfwidget/rtabwidget.h>
#include <base\selfwidget\rmessagebox.h>
#include <base/util/rutil.h>

#include "../testimagesitem.h"
#include "../../customwidget/customwidgetcontainer.h"

namespace Related {

	class TestImagesDetailDialog : public Base::DialogProxy
	{
		Q_OBJECT

	public:
		TestImagesDetailDialog(QWidget *parent = nullptr);
		~TestImagesDetailDialog();

		void  setSelectedImagesIndex(int index);

	private:
		void init();

	private:
		Base::RIconButton * m_leftImagesButt;
		Base::RIconButton * m_rightImagesButt;

		TestImagesItem * m_testImagesItem;
		int m_selectedImagesIndex;
	};

}//namespace Related 