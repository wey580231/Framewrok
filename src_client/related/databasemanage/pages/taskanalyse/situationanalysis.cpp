#include "situationanalysis.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWebEngineView>
#include <QWebChannel>

#include <core/file/programfilepath.h>
#include "../../customwidget/rcustomplot.h"

namespace Related {

	SituationAnalysis::SituationAnalysis(QWidget *parent)
		: AbstractPage(parent)
	{
		init();
	}

	SituationAnalysis::~SituationAnalysis()
	{
	}

	Related::PageType SituationAnalysis::getPageType() const
	{
		return Page_TaskAnalyse_SituationAnalysi;
	}

	void SituationAnalysis::init()
	{
		QWidget * mainWidget = new QWidget();

		m_mainGraph = new RCustomPlot();

		QWebEngineView * webView = new QWebEngineView();
		{
			Core::ProgramFilePath filePath;
			QDir tmpDir(filePath.programPath);

			QWebChannel* pWebChannel = new QWebChannel(this);
			webView->page()->setWebChannel(pWebChannel);

			QString urlPath = "file:///";
			urlPath += ("D:/code/qtcreator/build-ViewFrame-Desktop_Qt_5_12_10_MSVC2017_32bit-Release/Bin/rconfig/hw/mapdata/map_multicolor.html");

			webView->page()->setUrl(QUrl(urlPath));
		}

		QHBoxLayout * mainLayout = new QHBoxLayout();
		mainLayout->setContentsMargins(0, 0, 0, 0);
		mainLayout->addWidget(m_mainGraph);
		mainLayout->addWidget(webView);
		mainWidget->setLayout(mainLayout);

		QHBoxLayout * layout = new QHBoxLayout();
		layout->setContentsMargins(4, 4, 4, 4);
		layout->addWidget(mainWidget);
		this->setLayout(layout);
	}

} //namespace Related 