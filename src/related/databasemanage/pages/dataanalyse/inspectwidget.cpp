#include "inspectwidget.h"

InspectWidget::InspectWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	init();
}

InspectWidget::~InspectWidget()
{
}

void InspectWidget::slotSignalOut()
{
	emit signalSignalOut();
}

void InspectWidget::init()
{
	//[] ÕÛÏßÆµÆ×Í¼
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

	connect(ui.pushButton_6, SIGNAL(clicked()), this, SLOT(slotSignalOut()));

}
