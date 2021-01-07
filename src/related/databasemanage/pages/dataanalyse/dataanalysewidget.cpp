#include "dataanalysewidget.h"

namespace Related {

	DataAnalyseWidget::DataAnalyseWidget(QWidget *parent)
		: AbstractPage(parent)
	{
	}

	DataAnalyseWidget::~DataAnalyseWidget()
	{
	}

	Related::PageType DataAnalyseWidget::getPageType() const
	{
		return Page_DataAnalyse;
	}

	void DataAnalyseWidget::init()
	{

	}

} //namespace Related 