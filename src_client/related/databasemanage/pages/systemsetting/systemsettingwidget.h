/*!
 *  @brief     系统设置页面
 *  @details   提供对用户信息管理、系统设置等操作
 *  @author    wey
 *  @version   1.0
 *  @date      2021.01.07
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#pragma once

#include <QWidget>
#include <qstackedwidget.h>

#include <base\selfwidget\rtabwidget.h>
#include "../abstractpage.h"

#include "usermanagepage.h"
#include "othersettingpage.h"

namespace Related {

	class SystemSettingWidget : public AbstractPage
	{
		Q_OBJECT

	public:
		SystemSettingWidget(QWidget *parent = nullptr);
		~SystemSettingWidget();

		PageType getPageType() const;

		void prepareBringToTop() override;

	private:
		void init();

	private:
		Base::RTabWidget * m_tabWidget;

		UserManagePage * m_userManagePage;
		OtherSettingPage * m_otherSettingPage;
	};

} //namespace Related 