#include "testimagesdetaildialog.h"

#include <QDebug>

namespace Related {

	TestImagesDetailDialog::TestImagesDetailDialog(QWidget *parent)
		: Base::DialogProxy(parent),
		m_selectedImagesIndex(0)
	{
		setTitle(QStringLiteral("ÊÔÑéÍ¼Æ¬ÏêÇé"));
		setMinimumSize(900, 650);

		init();
	}

	TestImagesDetailDialog::~TestImagesDetailDialog()
	{

	}

	void TestImagesDetailDialog::setSelectedImagesIndex(int index)
	{
		m_selectedImagesIndex = index;
	}

	void TestImagesDetailDialog::init()
	{
		CustomWidgetContainer * imagesContainer = new CustomWidgetContainer();
		{
			m_testImagesItem = new TestImagesItem();
			m_testImagesItem->setImagesType(TestImagesItem::TI_Detail);

			QWidget * hWidget = new QWidget();
			QHBoxLayout * hLayout = new QHBoxLayout();
			hLayout->addWidget(m_testImagesItem);
			hLayout->setContentsMargins(0,0,0,0);
			imagesContainer->setLayout(hLayout);
			hWidget->setLayout(hLayout);
			imagesContainer->setContent(hWidget);
		}

// 		QGroupBox * groupBox = new QGroupBox();
// 		groupBox->setTitle(QStringLiteral("ÊÔÑéÍ¼Æ¬ÃèÊö"));

		QWidget *manWidget = new QWidget();
		QVBoxLayout * mainLayout = new QVBoxLayout();

		mainLayout->addWidget(imagesContainer, 6);
//		mainLayout->addWidget(groupBox, 1);
		mainLayout->setContentsMargins(4,4,4,4);
		manWidget->setLayout(mainLayout);
		this->setContentWidget(manWidget);
	}

}//namespace Related 