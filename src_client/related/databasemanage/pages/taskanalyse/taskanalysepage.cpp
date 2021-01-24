#include "taskanalysepage.h"

#include <QDebug>

namespace Related {

	TaskAnalysePage::TaskAnalysePage(QWidget *parent)
		: AbstractPage(parent)
	{
		init();
	}

	TaskAnalysePage::~TaskAnalysePage()
	{
	}

	Related::PageType TaskAnalysePage::getPageType() const
	{
		return Page_TaskDataAnalyse;
	}

	void TaskAnalysePage::respTabChanged(int page)
	{
		m_stackedWidget->setCurrentIndex(page);


	}

	void TaskAnalysePage::init()
	{
		m_tabWidget = new Base::RTabBar();
		connect(m_tabWidget, SIGNAL(currentIndexChanged(int)), this, SLOT(respTabChanged(int)));
		m_tabWidget->setFixedHeight(60);

		m_tabWidget->setTabAlignment(Base::RTabBar::AlignLeft);

		Base::RTabButton * dataOverviewButt = new Base::RTabButton(QStringLiteral("数据预览"));
		Base::RTabButton * audioDataButt = new Base::RTabButton(QStringLiteral("音频信息"));
		Base::RTabButton * ambientNoiseButt = new Base::RTabButton(QStringLiteral("环境噪声"));
		Base::RTabButton * hxjPlatformButt = new Base::RTabButton(QStringLiteral("HXJ平台信息"));

		m_tabWidget->addTabButton(dataOverviewButt, Tab_dataOverview);
		m_tabWidget->addTabButton(audioDataButt,	Tab_advInfo);
		m_tabWidget->addTabButton(ambientNoiseButt, Tab_ambientNoise);
		m_tabWidget->addTabButton(hxjPlatformButt,  Tab_hxjInfo);

		m_stackedWidget = new QStackedWidget();
		m_stackedWidget->setStyleSheet("background-color:rgba(0,0,0,0)");

		// 业务显示界面
		{
			m_dataOverviewWidget = new DataOverviewWidget(this);

			m_audioDataWidget = new AudioDataWidget(this);

			m_ambientNoiseDataWidget = new AmbientNoiseDataWidget(this);

			m_hxjPlatformDataWidget = new HXJPlatformInfoWidget(this);

			m_stackedWidget->addWidget(m_dataOverviewWidget);
			m_stackedWidget->addWidget(m_audioDataWidget);
			m_stackedWidget->addWidget(m_ambientNoiseDataWidget);
			m_stackedWidget->addWidget(m_hxjPlatformDataWidget);
		}

		QVBoxLayout * layout = new QVBoxLayout();
		layout->setContentsMargins(0, 0, 0, 0);
		layout->addWidget(m_tabWidget);
		layout->addWidget(m_stackedWidget);
		setLayout(layout);
	}

} //namespace Related 