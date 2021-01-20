/*!
 * @brief     任务记录页面
 * @details   提供值更记录、试验记录等页面
 * @author    wey
 * @version   1.0
 * @date      2021.01.20 09:16:07
 * @warning
 * @copyright NanJing RenGu.
 * @note
 */
 #pragma once

#include <QWidget>

#include "../abstractpage.h"

namespace Related {

	class TaskRecordPage : public AbstractPage
	{
		Q_OBJECT

	public:
		TaskRecordPage(QWidget *parent = nullptr);
		~TaskRecordPage();

		PageType getPageType() const;

	private:
		void init();
	};

} //namespace Related 