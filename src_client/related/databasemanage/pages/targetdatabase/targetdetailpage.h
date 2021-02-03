/*!
 * @brief     单个目标详情页面   
 * @author    wey
 * @version   1.0
 * @date      2021.02.03 10:42:17
 * @warning
 * @copyright NanJing RenGu.
 * @note
 */
 #pragma once

#include <QWidget>

#include "../abstractpage.h"

namespace Related {

	class TargetDetailPage : public AbstractPage
	{
		Q_OBJECT

	public:
		TargetDetailPage(QWidget *parent = nullptr);
		~TargetDetailPage();

		PageType getPageType() const;

	private:
		void init();

	};

} //namespace Related 