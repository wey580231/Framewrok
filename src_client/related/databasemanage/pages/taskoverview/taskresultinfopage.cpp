#include "taskresultinfopage.h"

#include <QDebug>

namespace Related {

	TaskResultInfoPage::TaskResultInfoPage(QWidget *parent )
		: QWidget(parent), m_curveIndex(0)
	{
		init();

	}

	TaskResultInfoPage::~TaskResultInfoPage()
	{
	}

	/*!
	 * @brief 设置平台结果信息
	 */
	void TaskResultInfoPage::setResultInfos(PlatformInfos info)
	{
		// 创建结果图元
		for (int i = 0; i < 15; i++) {
			QString path = QString("F:/RenGuCompany/Framewrok/src_client/core/resource/image/radar.png");

		
			TaskResultItem * item = new TaskResultItem();
			connect(item, SIGNAL(signalPictureIndex(int, QString)), this, SLOT(slotSeletePictureIndex(int, QString)));
			item->setMaximumSize(650, 550);
			item->setMinimumSize(680, 550);
			item-> setPictureIndex(i);
			item->setPicturePath(path);


			m_resultInfos.append(item);
		}
		createTaskResult();
	}

	void TaskResultInfoPage::slotLeftSwitchWidget()
	{
		qDebug() << "slotLeftSwitchWidget m_curveIndex" << m_curveIndex;
		if (m_curveIndex == 1 ) {
			m_curveIndex = m_resultInfos.size();

			QScrollBar  * t_ScrollBar = m_resultScrollArea->horizontalScrollBar();
			if (t_ScrollBar != NULL) {
				int max = t_ScrollBar->maximum();
				t_ScrollBar->setValue(max);
			}
		}
		else
		{
			m_curveIndex = m_curveIndex - 1;

			QScrollBar  * t_ScrollBar = m_resultScrollArea->horizontalScrollBar();
			if (t_ScrollBar != NULL) {
				t_ScrollBar->setValue(m_curveIndex);
			}

		}
		// 
		TaskResultItem * t_item = m_resultInfos.at(m_curveIndex - 1);


		//[] 刷新大图
		m_previewWidget->setPictureIndex(t_item->getPictureIndex());
		m_previewWidget->setPicturePath(t_item->getPicturePath());
		m_previewWidget->updateItem();

	}

	void TaskResultInfoPage::slotrightSwitchWidget()
	{
		qDebug() << "slotrightSwitchWidget  m_curveIndex" << m_curveIndex;
		if(m_curveIndex == m_resultInfos.size()){
			m_curveIndex = 1;
			QScrollBar  * t_ScrollBar = m_resultScrollArea->horizontalScrollBar();
			if (t_ScrollBar != NULL) {
				t_ScrollBar->setValue(0);
			}
		}
		else
		{
			m_curveIndex = m_curveIndex + 1;
			QScrollBar  * t_ScrollBar = m_resultScrollArea->horizontalScrollBar();
			if (t_ScrollBar != NULL) {
				t_ScrollBar->setValue(m_curveIndex);
			}
		}

		TaskResultItem * t_item = m_resultInfos.at(m_curveIndex - 1);


		//[] 刷新大图
		m_previewWidget->setPictureIndex(t_item->getPictureIndex());
		m_previewWidget->setPicturePath(t_item->getPicturePath());
		m_previewWidget->updateItem();

	}

	void  TaskResultInfoPage::slotSeletePictureIndex(int index, QString path)
	{
		m_curveIndex = index;

		//Base::RMessageBox * m_pMessageBox = new Base::RMessageBox();

		TaskResultItem *t_previewWidget = new TaskResultItem();
	//	QHBoxLayout *t_layput = new QHBoxLayout();
	//	t_layput->addWidget(t_previewWidget);
	//	RMessageBox:::setLayout(t_layput);
	
		//[] 刷新大图
		t_previewWidget->setPictureIndex(index);
		t_previewWidget->setPicturePath(path);
		t_previewWidget->update();
		//[]刷新其他状态为不选中
		for (int i = 0; i < m_resultInfos.size(); i++) {
			if (i != index) {
				TaskResultItem * t_item = m_resultInfos.at(i);
				t_item->setSelectedstatus(false);
				t_item->updateItem();
			}
			else if(i == index)
			{
				TaskResultItem * t_item = m_resultInfos.at(i);
				t_item->setSelectedstatus(true);
				t_item->updateItem();
			}
		}
		t_previewWidget->show();
	}

	void TaskResultInfoPage::init()
	{

		//[] 数据结果展示界面
		QWidget *btmWidget = new QWidget();
		{
			m_resultScrollArea = new QScrollArea();
			m_resultScrollArea->setStyleSheet("background-color:rgba(0,0,0,0)");
			m_resultScrollArea->setWidgetResizable(true);

			m_resultWidget = new QWidget();
			m_resultScrollArea->setWidget(m_resultWidget);

			QLabel *testLabel = new QLabel();
			testLabel->setText("1212121");

			QHBoxLayout *btmLayout = new QHBoxLayout();
			btmLayout->addWidget(m_resultScrollArea);
			btmLayout->setContentsMargins(0, 0, 0, 0);
			btmWidget->setLayout(btmLayout);
		}

		//[] 主界面布局
		QVBoxLayout *mainLayout = new QVBoxLayout();
// 		mainLayout->addWidget(titleWidget, 0);
// 		mainLayout->addWidget(topWidget, 2);
		mainLayout->addWidget(btmWidget, 1);
		mainLayout->setContentsMargins(4, 4, 4, 4);
		this->setLayout(mainLayout);
	}

	void TaskResultInfoPage::createTaskResult()
	{

		QHBoxLayout * vLayout = nullptr;
		if (m_resultWidget->layout() == nullptr) {
			vLayout = new QHBoxLayout();
			vLayout->setContentsMargins(0, 0, 0, 0);
			m_resultWidget->setLayout(vLayout);
		}
		else {
			vLayout = dynamic_cast<QHBoxLayout *>(m_resultWidget->layout());
		}

		for (int i = 0; i < m_resultInfos.size(); i++) {
			TaskResultItem * item = m_resultInfos.at(i);
		
			vLayout->addWidget(item);

		}
	}

}//namespace Related 