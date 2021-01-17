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

namespace Related {

	class OtherSettingPage : public QWidget
	{
		Q_OBJECT

	public:
		OtherSettingPage(QWidget *parent = nullptr);
		~OtherSettingPage();

	private:
		void init();

	};

} //namespace Related 