#pragma once

#include <QWidget>

namespace Related {

	class TaskReportPreviewPage : public QWidget
	{
		Q_OBJECT

	public:
		TaskReportPreviewPage(QWidget *parent = nullptr);
		~TaskReportPreviewPage();

	private:
		void init();

	};

}//namespace Related 
