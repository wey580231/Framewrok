/*!
 * @brief     »ŒŒÒ∏≈¿¿“≥√Ê   
 * @author    wey
 * @version   1.0
 * @date      2021.01.20 09:13:41
 * @warning
 * @copyright NanJing RenGu.
 * @note
 */
 #pragma once

#include <QWidget>

#include "../abstractpage.h"

namespace Related {

	class TaskOverViewPage : public AbstractPage
	{
		Q_OBJECT

	public:
		TaskOverViewPage(QWidget *parent = nullptr);
		~TaskOverViewPage();

		PageType getPageType() const;

	private:
		void init();


	};

} //namespace Related 