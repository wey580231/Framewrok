#include "DemonSpectrogramWidget.h"

#include <QDebug>

DemonSpectrogramWidget::DemonSpectrogramWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	init();
}

DemonSpectrogramWidget::~DemonSpectrogramWidget()
{
}

void DemonSpectrogramWidget::init()
{
	//[] 折线频谱图
	SpectrogramWidget *m_pSpectrogramWidget1 = new SpectrogramWidget();
	SpectrogramWidget *m_pSpectrogramWidget2 = new SpectrogramWidget();
	QVBoxLayout *t_pQHBoxLayout1 = new QVBoxLayout();
	t_pQHBoxLayout1->addWidget(m_pSpectrogramWidget1);
	t_pQHBoxLayout1->addWidget(m_pSpectrogramWidget2);
	t_pQHBoxLayout1->setContentsMargins(0, 0, 0, 0);
	t_pQHBoxLayout1->setSpacing(0);
	ui.widget_3->setLayout(t_pQHBoxLayout1);

	SpectrogramWidget *m_pSpectrogramWidget3 = new SpectrogramWidget();
	SpectrogramWidget *m_pSpectrogramWidget4 = new SpectrogramWidget();
	QVBoxLayout *t_pQHBoxLayout = new QVBoxLayout();
	t_pQHBoxLayout->addWidget(m_pSpectrogramWidget3);
	t_pQHBoxLayout->addWidget(m_pSpectrogramWidget4);
	t_pQHBoxLayout->setContentsMargins(0, 0, 0, 0);
	t_pQHBoxLayout->setSpacing(0);
	ui.widget_4->setLayout(t_pQHBoxLayout);


	//[] 数据样本管理界面
	m_datasamplepage = new Related::datasamplepage();
	connect(m_datasamplepage, SIGNAL(signalDataGather()), this, SIGNAL(signalDataGather()));
	connect(m_datasamplepage, SIGNAL(signalInspectData()), this, SIGNAL(signalInspectAction()));

	QVBoxLayout *t_pQHBoxLayout3 = new QVBoxLayout();
	t_pQHBoxLayout3->setContentsMargins(0, 0, 0, 0);
	t_pQHBoxLayout3->setSpacing(0);
	t_pQHBoxLayout3->addWidget(m_datasamplepage);

	ui.widget_5->setLayout(t_pQHBoxLayout3);



	t_candidateDataPage = new Related::CandidateDataPage();
	QHBoxLayout *t_HLayout = new QHBoxLayout();
	t_HLayout->addWidget(t_candidateDataPage);
	t_HLayout->setContentsMargins(0, 0, 0, 0);
	t_HLayout->setSpacing(0);
	ui.tab_2->setLayout(t_HLayout);

	t_candidateDataPage3 = new Related::CandidateDataPage();
	QHBoxLayout *t_HLayout3 = new QHBoxLayout();
	t_HLayout3->addWidget(t_candidateDataPage3);
	t_HLayout3->setContentsMargins(0, 0, 0, 0);
	t_HLayout3->setSpacing(0);
	ui.tab_3->setLayout(t_HLayout3);


}

