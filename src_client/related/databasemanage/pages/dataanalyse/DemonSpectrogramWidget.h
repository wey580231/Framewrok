#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTableWidget>

#include "ui_DemonSpectrogramWidget.h"

#include "SpectrogramWidget.h"

#include "datasamplepage.h"
#include "candidatedatapage.h"


class DemonSpectrogramWidget : public QWidget
{
	Q_OBJECT
public:
	DemonSpectrogramWidget(QWidget *parent = Q_NULLPTR);
	~DemonSpectrogramWidget();

signals:
	void signalDataGather();
	void signalInspectAction();

private:
	void  init();

private:
	Ui::DemonSpectrogramWidget ui;
	Related::datasamplepage *m_datasamplepage;
	Related::CandidateDataPage *t_candidateDataPage;
	Related::CandidateDataPage *t_candidateDataPage3;
};
