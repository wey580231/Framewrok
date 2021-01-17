/*!
 * @brief     ÏµÍ³µÇÂ¼Ò³Ãæ
 * @author    wey
 * @version   1.0
 * @date      2021.01.16 17:27:14
 * @warning
 * @copyright NanJing RenGu.
 * @note
 */
#pragma once

#include <QWidget>

namespace Related {

	class LoginPage : public QWidget
	{
		Q_OBJECT

	public:
		LoginPage(QWidget *parent = nullptr);
		~LoginPage();

	private:
		void init();
	};

} //namespace Related 