#pragma once

#include <QObject>
#include <QWidget>
#include <QtCharts>
#include <QChartView>
#include <QLineSeries>
#include <QVBoxLayout>
#include <QPushButton>
#include <QPalette>
#include <QApplication>

#include <iostream>

class SpectrogramWidget : public QWidget
{
	Q_OBJECT
public:
	SpectrogramWidget(QWidget *parent = nullptr);
	~SpectrogramWidget();


private:
};
