#include "hxjplatforminfowidget.h"

#include <QDebug>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QLabel>

#include "../../utils/util.h"
#include "../../customwidget/customwidgetcontainer.h"
#include "../../customwidget/rcustomplot.h"

namespace Related {

	HXJPlatformInfoWidget::HXJPlatformInfoWidget(QWidget *parent)
		: AbstractPage(parent)
	{
		init();
	}

	HXJPlatformInfoWidget::~HXJPlatformInfoWidget()
	{

	}

	PageType HXJPlatformInfoWidget::getPageType() const
	{
		return PageType();
	}

	void HXJPlatformInfoWidget::prepareBringToTop()
	{
	}

	void HXJPlatformInfoWidget::init()
	{
		auto createCheckBox = [](QString boxName,int fixedWidth = 60) {
			QCheckBox * box = new QCheckBox;
			box->setFixedWidth(fixedWidth);
			box->setText(boxName);
			return box;
		};

		CustomWidgetContainer * typeContainer = new CustomWidgetContainer();
		{
			QWidget * dataTypeWidget = new QWidget();
			//dataTypeWidget->setFixedHeight(50);
			QLabel * typeLabel = new QLabel();
			typeLabel->setText(QStringLiteral("数据类型:"));

			QHBoxLayout * dataTypeLayout = new QHBoxLayout();
			dataTypeLayout->addWidget(typeLabel);
			dataTypeLayout->addWidget(createCheckBox(QStringLiteral("温度")));
			dataTypeLayout->addWidget(createCheckBox(QStringLiteral("压力")));
			dataTypeLayout->addWidget(createCheckBox(QStringLiteral("深度")));
			dataTypeLayout->addWidget(createCheckBox(QStringLiteral("航向角")));
			dataTypeLayout->addWidget(createCheckBox(QStringLiteral("俯仰角")));
			dataTypeLayout->addWidget(createCheckBox(QStringLiteral("横滚角")));
			dataTypeLayout->addStretch(1);

			dataTypeWidget->setLayout(dataTypeLayout);

			typeContainer->setContent(dataTypeWidget);
		}

		CustomWidgetContainer * workContainer = new CustomWidgetContainer();
		{
			QWidget * workStateWidget = new QWidget();
			//workStateWidget->setFixedHeight(40);

			QLabel * typeLabel = new QLabel();
			typeLabel->setText(QStringLiteral("工作状态:"));

			QHBoxLayout * workLayout = new QHBoxLayout();
			workLayout->addWidget(typeLabel);
			workLayout->addWidget(createCheckBox(QStringLiteral("上浮状态"),90));
			workLayout->addWidget(createCheckBox(QStringLiteral("下潜状态"), 90));
			workLayout->addWidget(createCheckBox(QStringLiteral("水下航行状态"), 110));
			workLayout->addStretch(1);

			workStateWidget->setLayout(workLayout);
			workContainer->setContent(workStateWidget);
		}

		CustomWidgetContainer * container = new CustomWidgetContainer();
		container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
		{
			RCustomPlot * plot = new RCustomPlot();
			container->setContent(plot);
		}

		QVBoxLayout * vlayout = new QVBoxLayout();
		vlayout->setContentsMargins(4, 4, 4, 4);
		vlayout->addWidget(typeContainer);
		vlayout->addWidget(workContainer);
		vlayout->addWidget(container);
		setLayout(vlayout);
	}

}//namespace Related 
