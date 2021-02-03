/*!
 * @brief      ±”Ú≤®–Œ∑÷Œˆ   
 * @author    wey
 * @version   1.0
 * @date      2021.02.01 13:47:19
 * @warning
 * @copyright NanJing RenGu.
 * @note
 */
 #pragma once

#include <QWidget>

#include <base\selfwidget\iconbutton.h>
#include "../abstractpage.h"

namespace Related {

	class TimeDomainAnalyse : public QWidget
	{
		Q_OBJECT

	public:
		TimeDomainAnalyse(QWidget *parent);
		~TimeDomainAnalyse();

		PageType getPageType() const;

	private:
		void init();

	};

} //namespace Related