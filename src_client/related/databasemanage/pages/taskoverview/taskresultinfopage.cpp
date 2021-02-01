#include "taskresultinfopage.h"

#include <QDebug>

namespace Related {

	TaskResultInfoPage::TaskResultInfoPage(QWidget *parent )
		: QWidget(parent)
	{
		m_imagesItems.clear();
		init();
		createImagesItem();
	}

	TaskResultInfoPage::~TaskResultInfoPage()
	{

	}

	void TaskResultInfoPage::init()
	{
		QWidget *btmWidget = new QWidget();
		{
			m_resultScrollArea = new QScrollArea();
			m_resultScrollArea->setStyleSheet("background-color:rgba(0,0,0,0)");
			m_resultScrollArea->setWidgetResizable(true);

			m_imagesWidget = new QWidget();
			m_imagesWidget->setStyleSheet("background-color:rgba(0,0,0,0)");
			m_resultScrollArea->setWidget(m_imagesWidget);

			QHBoxLayout *btmLayout = new QHBoxLayout();
			btmLayout->addWidget(m_resultScrollArea);
			btmLayout->setContentsMargins(0, 0, 0, 0);
			btmWidget->setLayout(btmLayout);
		}

		//[] 主界面布局
		QVBoxLayout * mainLayout = new QVBoxLayout();
		mainLayout->addWidget(btmWidget);
		mainLayout->setContentsMargins(4, 4, 4, 4);
		this->setLayout(mainLayout);
	}

	void TaskResultInfoPage::updateTestImages()
	{
		QGridLayout * gLayout = nullptr;

		if (m_imagesWidget->layout() == nullptr) {
			gLayout = new QGridLayout();
			gLayout->setContentsMargins(0, 0, 0, 0);
			m_imagesWidget->setLayout(gLayout);
		}
		else {
			gLayout = dynamic_cast<QGridLayout *>(m_imagesWidget->layout());
			for (int i = gLayout->count() - 1; i >= 0; i--) {
				if (gLayout->itemAt(i)->widget()) {
					delete gLayout->takeAt(i);
				}
			}
		}

		for (int i = 0; i < m_imagesItems.size(); i++) {
			int row = i / 5;
			int column = i % 5;
			gLayout->addWidget(m_imagesItems.at(i), row, column, 1, 1);
		}
	}

	void TaskResultInfoPage::createImagesItem()
	{
		for (int i = 0; i < 15; i++) {
			TestImagesItem * item = new TestImagesItem();
			m_imagesItems.append(item);
		}
		updateTestImages();
	}

}//namespace Related 