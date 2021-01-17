#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTableWidget>

#include "ui_gatherwidget.h"

class gatherwidget : public QWidget
{
	Q_OBJECT

public:
	gatherwidget(QWidget *parent = Q_NULLPTR);
	~gatherwidget();



signals:
	void  signalSignalOut();

private slots:
	void slotSignalOut();

private:
	void init();
private:
	Ui::gatherwidget ui;
};
