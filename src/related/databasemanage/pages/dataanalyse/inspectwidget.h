/*!
 *  @brief     数核查分析界面
 *  @details   
 *  @author    yzg
 *  @version   1.0
 *  @date      2021.01.12
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#pragma once

#include <QWidget>
#include "ui_inspectwidget.h"

#include "SpectrogramWidget.h"

class InspectWidget : public QWidget
{
	Q_OBJECT

public:
	InspectWidget(QWidget *parent = Q_NULLPTR);
	~InspectWidget();

signals:
	void signalSignalOut();

private slots:
	void slotSignalOut();

private:
	void  init();

private:
	Ui::inspectwidget ui;
};
