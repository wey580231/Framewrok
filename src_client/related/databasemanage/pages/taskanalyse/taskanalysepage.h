/*!
 * @brief     任务分析页面  
 * @author    wey
 * @version   1.0
 * @date      2021.01.20 09:18:29
 * @warning
 * @copyright NanJing RenGu.
 * @note
 */
 #pragma once

#include <QWidget>

#include "../abstractpage.h"

namespace Related {

	class TaskAnalysePage : public AbstractPage
	{
		Q_OBJECT

	public:
		TaskAnalysePage(QWidget *parent = nullptr);
		~TaskAnalysePage();

		PageType getPageType() const;

	private:
		void init();

	};

} //namespace Related 