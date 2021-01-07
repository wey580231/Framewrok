#pragma once

#include <QWidget>

#include "../datastruct.h"

namespace Related {

	class AbstractPage : public QWidget
	{
		Q_OBJECT

	public:
		AbstractPage(QWidget *parent);
		~AbstractPage();

		virtual PageType getPageType() const = 0;

	};

} //namespace Related 