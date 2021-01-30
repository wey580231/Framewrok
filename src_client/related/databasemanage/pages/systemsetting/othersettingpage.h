/*!
 * @brief     其它参数设置页面   
 * @author    wey
 * @version   1.0
 * @date      2021.01.16 17:17:01
 * @warning
 * @copyright NanJing RenGu.
 * @note
 */
#pragma once

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>

#include <base/selfwidget/rmessagebox.h>

#include "../abstractpage.h"

#include "detectplatformpage.h"

namespace Related {

	class OtherSettingPage : public AbstractPage
	{
		Q_OBJECT

	public:
		OtherSettingPage(QWidget *parent = nullptr);
		~OtherSettingPage();

		PageType getPageType() const;

		void prepareBringToTop();

	private slots:

	private:
		void init();

	private:
		DetectPlatformPage * m_detectPlatformPage;						/*!< 侦测平台 */

	};

} //namespace Related 