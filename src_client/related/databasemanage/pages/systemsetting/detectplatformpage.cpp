#include "detectplatformpage.h"

#include <QDebug>

#include "../../customwidget/customwidgetcontainer.h"
#include "../../net/datanetconnector.h"
#include "../../net/signaldispatch.h"

namespace Related {

	DetectPlatformPage::DetectPlatformPage(QWidget *parent)
		: AbstractPage(parent),
		m_operationToolsPage(nullptr),
		m_firstLoadData(true),
		m_selectedIndex(TABLEMODEL_MAX_INDEX),
		m_selectedModel(false)
	{
		m_detectPlatformsMap.clear();

		init();
		initConnect();
	}

	DetectPlatformPage::~DetectPlatformPage()
	{
	}

	PageType DetectPlatformPage::getPageType() const
	{
		return Page_Setting_DetectPlatformPage;
	}

	void DetectPlatformPage::prepareBringToTop()
	{
		if (m_firstLoadData) {
			refreshCurrDetectPlatform();
			m_firstLoadData = false;
		}
	}

	void DetectPlatformPage::respToolButtPressed(OperationToolsPage::ButtType type)
	{
		switch (type)
		{
		case OperationToolsPage::Butt_Add: {
			DetectPlatformEditDialog dialog(this);
			dialog.setEditType(DetectPlatformEditDialog::E_DetectPlatform);
			if (dialog.exec() == QDialog::Accepted) {
				refreshCurrDetectPlatform();
			}
		}
			break;
		case OperationToolsPage::Butt_Delete: {
			QString name = m_DetectPlatformComboBox->currentText();
			if (m_detectPlatformsMap.contains(name)) {
				int detectId = m_detectPlatformsMap.find(name).value();
				deleteDetectPlatform(detectId);
			}
		}
			break;
		default:
			break;
		}
	}

	void DetectPlatformPage::slotActivated(const QString & text)
	{
		m_listModel->clear();
		if (m_detectPlatformsMap.contains(text)) {
			int detectId = m_detectPlatformsMap.find(text).value();
			if (m_detectPlatformSubtypesResponseMap.contains(detectId)) {
				Datastruct::LoadAllDetectPlatformSubtypesResponse t_response = m_detectPlatformSubtypesResponseMap.find(detectId).value();
				for (int i = 0; i < t_response.m_detectPlatformSubtypeInfos.size(); i++) {
					Datastruct::DetectPlatformSubtypeEntityData data = t_response.m_detectPlatformSubtypeInfos.at(i);
					QStandardItem *item = new QStandardItem(data.name);
					m_listModel->appendRow(item);
				}
			}
		}
	}

	void DetectPlatformPage::slotCreateNewDetectPlatformSubtypeTriggered()
	{
		QString  detectPlatformName = m_DetectPlatformComboBox->currentText();
		if (m_detectPlatformsMap.contains(detectPlatformName)) {
			int detectPlatformId = m_detectPlatformsMap.find(detectPlatformName).value();

			DetectPlatformEditDialog dialog(this);
			dialog.setEditType(DetectPlatformEditDialog::E_DetectPlatformSubtype);
			dialog.setPlatformId(detectPlatformId);

			if (dialog.exec() == QDialog::Accepted) {
				refreshCurrDetectPlatform();
			}
		}
	}

	void DetectPlatformPage::slotDetectPlatformSubtypeClicked(QModelIndex index)
	{
		m_selectedModelIndex = index;
		m_selectedIndex = index.row();
		m_selectedModel = true;
	}

	void DetectPlatformPage::slotDeleteDetectPlatformSubtypeTriggered()
	{

		QVariant data = m_selectedModelIndex.data(0);
		QString name = data.toString();
		deleteDetectPlatformSubtype(name);
// 		QString  detectPlatformName = m_DetectPlatformComboBox->currentText();
// 		if (m_detectPlatformsMap.contains(detectPlatformName)) {
// 			int detectPlatformId = m_detectPlatformsMap.find(detectPlatformName).value();
// 
// 			if (m_detectPlatformSubtypesResponseMap.contains(detectPlatformId)) {
// 				Datastruct::LoadAllDetectPlatformSubtypesResponse responseList = m_detectPlatformSubtypesResponseMap.find(detectPlatformId).value();
// 				for (int i = 0; i < responseList.m_detectPlatformSubtypeInfos.size(); i++) {
// 
// 				}
// 			}
// 		}


		m_selectedModel = false;
	}

	void DetectPlatformPage::processQueryAllDetectPlatformResponse(const Datastruct::LoadAllDetectPlatformsResponse & response)
	{
		m_detectPlatformsList = response;
		m_DetectPlatformComboBox->clear();
		m_detectPlatformsMap.clear();

		if (response.m_detectPlatformCount > 0) {
			for (int i = 0; i < response.m_detectPlatformInfos.size() > 0;i++) {
				Datastruct::DetectPlatformEntityData data = response.m_detectPlatformInfos.at(i);
				m_DetectPlatformComboBox->addItem(data.name);
				m_detectPlatformsMap.insert(data.name, data.id);
				//[] 获取子类型
				refreshCurrDetectPlatformSubtype(data.id);
			}
		}
	}

	void DetectPlatformPage::processDetectPlatformDeleteResponse(const Datastruct::DetectPlatformDeleteResponse & response)
	{
		if (response.m_deleteResult) 
		{
			refreshCurrDetectPlatform();
		}
		else
		{

		}
	}

	void DetectPlatformPage::processQueryAllDetectPlatformSubtypeResponse(const Datastruct::LoadAllDetectPlatformSubtypesResponse & response)
	{
		if (response.m_detectPlatformSubtypeCount > 0) {
			m_detectPlatformSubtypesResponseMap.insert(response.m_detectId, response);

			//是否刷新界面
			m_listModel->clear();
			QString name =	m_DetectPlatformComboBox->currentText();
			if (m_detectPlatformsMap.contains(name)) {
				int detectId = m_detectPlatformsMap.find(name).value();

				if (m_detectPlatformSubtypesResponseMap.contains(detectId)) {
			
					Datastruct::LoadAllDetectPlatformSubtypesResponse t_response = m_detectPlatformSubtypesResponseMap.find(detectId).value();

					for (int i = 0; i < t_response.m_detectPlatformSubtypeInfos.size(); i++) {
						Datastruct::DetectPlatformSubtypeEntityData data = t_response.m_detectPlatformSubtypeInfos.at(i);
						QStandardItem *item = new QStandardItem(data.name);
						m_listModel->appendRow(item);
					}
				}
			}
		}
	}

	void DetectPlatformPage::processDetectPlatformSubtypeDeleteResponse(const Datastruct::DetectPlatformSubtypeDeleteResponse & response)
	{
		refreshCurrDetectPlatform();
	}

	void DetectPlatformPage::contextMenuEvent(QContextMenuEvent * event)
	{
		QMenu *menu = new QMenu(this);
		QAction * newAction = new QAction();
		newAction->setText(QStringLiteral("新增平台亚型"));
		connect(newAction, SIGNAL(triggered()), this, SLOT(slotCreateNewDetectPlatformSubtypeTriggered()));
		QAction * deleteAction = new QAction();
		deleteAction->setText(QStringLiteral("删除"));
		connect(deleteAction, SIGNAL(triggered()), this, SLOT(slotDeleteDetectPlatformSubtypeTriggered()));

		if (m_selectedModel == false) {
			menu->addAction(newAction);
		}
		else
		{
			menu->addAction(deleteAction);
		}

		menu->exec(QCursor::pos());
	}

	void DetectPlatformPage::init()
	{
		CustomWidgetContainer * cwidget = new CustomWidgetContainer();
		{
			m_operationToolsPage = new OperationToolsPage();
			m_operationToolsPage->setButtVisible(OperationToolsPage::Butt_Edit, false);
			m_operationToolsPage->setButtVisible(OperationToolsPage::Butt_Refresh, false);
			connect(m_operationToolsPage, SIGNAL(buttPressed(OperationToolsPage::ButtType)), 
				this, SLOT(respToolButtPressed(OperationToolsPage::ButtType)));
			QLabel * detectPlatformLabel = new QLabel();
			detectPlatformLabel->setText(QStringLiteral("侦测平台："));
			m_DetectPlatformComboBox = new QComboBox();
			connect(m_DetectPlatformComboBox, SIGNAL(activated(QString)), this, SLOT(slotActivated(QString)));

			QLabel * detectPlatformSubtypeLabel = new QLabel();
			detectPlatformSubtypeLabel->setText(QStringLiteral("侦测平台亚型："));
		
			m_DetectPlatformSubtypeListView = new QListView();
			connect(m_DetectPlatformSubtypeListView, SIGNAL(clicked(QModelIndex)), this, SLOT(slotDetectPlatformSubtypeClicked(QModelIndex)));

			m_listModel = new QStandardItemModel();

			m_DetectPlatformSubtypeListView->setModel(m_listModel);

			QHBoxLayout *hLayout = new QHBoxLayout();
			hLayout->addWidget(detectPlatformLabel);
			hLayout->addWidget(m_DetectPlatformComboBox);


			QWidget * widget = new QWidget();
			QVBoxLayout *vLayout = new QVBoxLayout();
			vLayout->addLayout(hLayout);
			vLayout->addWidget(m_DetectPlatformSubtypeListView);
			vLayout->addWidget(m_operationToolsPage);
			widget->setLayout(vLayout);
			cwidget->setContent(widget);
		}
		QVBoxLayout *mainLayout = new QVBoxLayout();
		mainLayout->setContentsMargins(4, 4, 4, 4);
		mainLayout->addWidget(cwidget);
		setLayout(mainLayout);
	}

	void DetectPlatformPage::initConnect()
	{
		connect(SignalDispatch::instance(), SIGNAL(respQueryAllDetectPlatformsResponse(const Datastruct::LoadAllDetectPlatformsResponse &)),
			this, SLOT(processQueryAllDetectPlatformResponse(const Datastruct::LoadAllDetectPlatformsResponse &)));

		connect(SignalDispatch::instance(), SIGNAL(respDetectPlatformDeleteResponse(const Datastruct::DetectPlatformDeleteResponse &)),
			this, SLOT(processDetectPlatformDeleteResponse(const Datastruct::DetectPlatformDeleteResponse &)));

		connect(SignalDispatch::instance(), SIGNAL(respQueryAllDetectPlatformSubtypesResponse(const Datastruct::LoadAllDetectPlatformSubtypesResponse &)),
			this, SLOT(processQueryAllDetectPlatformSubtypeResponse(const Datastruct::LoadAllDetectPlatformSubtypesResponse &)));

		connect(SignalDispatch::instance(), SIGNAL(respDetectPlatformSubtypeDeleteResponse(const Datastruct::DetectPlatformSubtypeDeleteResponse &)),
			this, SLOT(processDetectPlatformSubtypeDeleteResponse(const Datastruct::DetectPlatformSubtypeDeleteResponse &)));
	}

	void DetectPlatformPage::refreshCurrDetectPlatform()
	{
		Datastruct::LoadAllDetectPlatformsRequest request;
		DataNetConnector::instance()->write(request);
	}

	void DetectPlatformPage::deleteDetectPlatform(int id)
	{
		Datastruct::DetectPlatformDeleteRequest request;
		request.m_id = id;
		DataNetConnector::instance()->write(request);
	}

	void DetectPlatformPage::refreshCurrDetectPlatformSubtype(int detectId)
	{
		Datastruct::LoadAllDetectPlatformSubtypesRequest request;
		request.m_detectId = detectId;
		DataNetConnector::instance()->write(request);
	}

	void DetectPlatformPage::deleteDetectPlatformSubtype(QString name)
	{
		Datastruct::DetectPlatformSubtypeDeleteRequest request;
		request.m_name = name;
		DataNetConnector::instance()->write(request);
	}

}//namespace Related 