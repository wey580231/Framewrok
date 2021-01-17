#include "dataanalysewidget.h"

namespace Related {

	DataAnalyseWidget::DataAnalyseWidget(QWidget *parent)
		: AbstractPage(parent)
	{
		init();
	}

	DataAnalyseWidget::~DataAnalyseWidget()
	{
	}

	Related::PageType DataAnalyseWidget::getPageType() const
	{
		return Page_DataAnalyse;
	}

	void DataAnalyseWidget::slotDataGather()
	{
		m_stackedWidget->setCurrentIndex(1);
	}

	void DataAnalyseWidget::slotSignalOut()
	{
		m_stackedWidget->setCurrentIndex(0);
	}

	void DataAnalyseWidget::slotInspectSignalOut()
	{
		m_stackedWidget->setCurrentIndex(2);
	}

	void DataAnalyseWidget::init()
	{
		DemonSpectrogramWidget * m_pDemonSpectrogramWidget = new DemonSpectrogramWidget();
		connect(m_pDemonSpectrogramWidget, SIGNAL(signalDataGather()), this, SLOT(slotDataGather()));
		connect(m_pDemonSpectrogramWidget, SIGNAL(signalInspectAction()), this, SLOT(slotInspectSignalOut()));
		
		gatherwidget  *t_gatherwidget = new gatherwidget();

		connect(t_gatherwidget, SIGNAL(signalSignalOut()), this, SLOT(slotSignalOut()));

		InspectWidget  *t_InspectWidget = new InspectWidget();
		connect(t_InspectWidget, SIGNAL(signalSignalOut()), this, SLOT(slotSignalOut()));

		m_stackedWidget = new QStackedWidget();
		m_stackedWidget->addWidget(m_pDemonSpectrogramWidget);
		m_stackedWidget->addWidget(t_gatherwidget);
		m_stackedWidget->addWidget(t_InspectWidget);

		QVBoxLayout *m_pHBoxLayout = new QVBoxLayout();
		m_pHBoxLayout->addWidget(m_stackedWidget);
		this->setLayout(m_pHBoxLayout);
	}

} //namespace Related 