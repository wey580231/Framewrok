#include "ambientnoisedatawidget.h"

#include <QDebug>
#include <QRadioButton>

#include "../../utils/util.h"
#include "../../customwidget/customwidgetcontainer.h"
#include "../../customwidget/rcustomplot.h"

namespace Related {

	AmbientNoiseDataWidget::AmbientNoiseDataWidget(QWidget *parent)
		: AbstractPage(parent)
	{
		init();
	}

	AmbientNoiseDataWidget::~AmbientNoiseDataWidget()
	{
	}

	PageType AmbientNoiseDataWidget::getPageType() const
	{
		return PageType();
	}

	void AmbientNoiseDataWidget::init()
	{
		auto createCheckBox = [](QString boxName, int fixedWidth = 60) {
			QRadioButton * box = new QRadioButton;
			box->setFixedWidth(fixedWidth);
			box->setText(boxName);
			return box;
		};

		CustomWidgetContainer * timeContainer = new CustomWidgetContainer();
		{
			QWidget * dataTypeWidget = new QWidget();
			QLabel * typeLabel = new QLabel();
			typeLabel->setText(QStringLiteral("时间间隔:"));

			QHBoxLayout * dataTypeLayout = new QHBoxLayout();
			dataTypeLayout->addWidget(typeLabel);
			dataTypeLayout->addWidget(createCheckBox(QStringLiteral("1小时")));
			dataTypeLayout->addWidget(createCheckBox(QStringLiteral("6小时")));
			dataTypeLayout->addWidget(createCheckBox(QStringLiteral("8小时")));
			dataTypeLayout->addStretch(1);

			dataTypeWidget->setLayout(dataTypeLayout);
			timeContainer->setContent(dataTypeWidget);
		}

		CustomWidgetContainer * graphContainer = new CustomWidgetContainer();
		graphContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
		{
			RCustomPlot * plot = new RCustomPlot();
			graphContainer->setContent(plot);
		}

		QVBoxLayout * vlayout = new QVBoxLayout();
		vlayout->setContentsMargins(4, 4, 4, 4);
		vlayout->addWidget(timeContainer);
		vlayout->addWidget(graphContainer);
		setLayout(vlayout);
	}

}//namespace Related 