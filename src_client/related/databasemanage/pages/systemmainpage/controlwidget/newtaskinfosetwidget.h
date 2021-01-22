#pragma once

#include <QWidget>
#include "ui_newtaskinfosetwidget.h"

	class NewTaskInfoSetWidget : public QWidget
	{
		Q_OBJECT

	public:
		NewTaskInfoSetWidget(QWidget *parent = Q_NULLPTR);
		~NewTaskInfoSetWidget();

	private:
		Ui::NewTaskInfoSetWidget ui;
	};

