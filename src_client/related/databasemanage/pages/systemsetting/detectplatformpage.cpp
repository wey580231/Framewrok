#include "detectplatformpage.h"

#include <QDebug>

#include "../../customwidget/customwidgetcontainer.h"
#include "../../net/datanetconnector.h"
#include "../../net/signaldispatch.h"

namespace Related {

	DetectPlatformPage::DetectPlatformPage(QWidget *parent)
		: AbstractPage(parent),
		m_operationToolsPage(nullptr),
		m_firstLoadData(true)
	{
		m_detectPlatformsMap.clear();
		m_detectPlatformSubtypesMap.clear();

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

	void DetectPlatformPage::processDetectPlatformCreateResponse(const Datastruct::DetectPlatformCreateResponse & response)
	{

	}

	void DetectPlatformPage::processDetectPlatformDeleteResponse(const Datastruct::DetectPlatformDeleteResponse & response)
	{
	}

	void DetectPlatformPage::processDetectPlatformModifyResponse(const Datastruct::DetectPlatformModifyResponse & response)
	{
	}

	void DetectPlatformPage::processQueryAllDetectPlatformSubtypeResponse(const Datastruct::LoadAllDetectPlatformSubtypesResponse & response)
	{
		if (response.m_detectPlatformSubtypeCount > 0) {
			if (m_detectPlatformSubtypesMap.contains(response.m_detectId)) {
				QMap<QString, int > t_map = m_detectPlatformSubtypesMap.find(response.m_detectId).value();
				t_map.clear();

				for (int i = 0; i < response.m_detectPlatformSubtypeInfos.size(); i++) {
					Datastruct::DetectPlatformSubtypeEntityData data = response.m_detectPlatformSubtypeInfos.at(i);
					t_map.insert(data.name, data.id);
				}
				m_detectPlatformSubtypesMap.insert(response.m_detectId, t_map);
				m_detectPlatformSubtypesResponseMap.insert(response.m_detectId, response);
			}

			//是否刷新界面
			QString name =	m_DetectPlatformComboBox->currentText();
// 			if (m_detectPlatformsMap.contains(name)) {
// 				int detectId = 
// 			}

		}
	}

	void DetectPlatformPage::processDetectPlatformSubtypeCreateResponse(const Datastruct::DetectPlatformSubtypeCreateResponse & response)
	{
	}

	void DetectPlatformPage::processDetectPlatformSubtypeDeleteResponse(const Datastruct::DetectPlatformSubtypeDeleteResponse & response)
	{
	}

	void DetectPlatformPage::processDetectPlatformSubtypeModifyResponse(const Datastruct::DetectPlatformSubtypeModifyResponse & response)
	{
	}

	void DetectPlatformPage::respToolButtPressed(OperationToolsPage::ButtType type)
	{
		switch (type)
		{
		case OperationToolsPage::Butt_Add: {

		}
			break;
		case OperationToolsPage::Butt_Delete: {

		}
			break;
		case OperationToolsPage::Butt_Edit: {
		}
			break;
		case OperationToolsPage::Butt_Refresh: {

		}
			break;
		default:
			break;
		}
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

			QLabel * detectPlatformSubtypeLabel = new QLabel();
			detectPlatformSubtypeLabel->setText(QStringLiteral("侦测平台亚型："));
		
			m_DetectPlatformSubtypeListView = new QListView();

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

		connect(SignalDispatch::instance(), SIGNAL(respDetectPlatformCreateResponse(const Datastruct::DetectPlatformCreateResponse &)),
			this, SLOT(processDetectPlatformCreateResponse(const Datastruct::DetectPlatformCreateResponse &)));

		connect(SignalDispatch::instance(), SIGNAL(respDetectPlatformDeleteResponse(const Datastruct::DetectPlatformDeleteResponse &)),
			this, SLOT(processDetectPlatformDeleteResponse(const Datastruct::DetectPlatformDeleteResponse &)));

		connect(SignalDispatch::instance(), SIGNAL(respDetectPlatformModifyResponse(const Datastruct::DetectPlatformModifyResponse &)),
			this, SLOT(processDetectPlatformModifyResponse(const Datastruct::DetectPlatformModifyResponse &)));


		connect(SignalDispatch::instance(), SIGNAL(respQueryAllDetectPlatformSubtypesResponse(const Datastruct::LoadAllDetectPlatformSubtypesResponse &)),
			this, SLOT(processQueryAllDetectPlatformSubtypeResponse(const Datastruct::LoadAllDetectPlatformSubtypesResponse &)));

		connect(SignalDispatch::instance(), SIGNAL(respDetectPlatformSubtypeCreateResponse(const Datastruct::DetectPlatformSubtypeCreateResponse &)),
			this, SLOT(processDetectPlatformSubtypeCreateResponse(const Datastruct::DetectPlatformSubtypeCreateResponse &)));

		connect(SignalDispatch::instance(), SIGNAL(respDetectPlatformSubtypeDeleteResponse(const Datastruct::DetectPlatformSubtypeDeleteResponse &)),
			this, SLOT(processDetectPlatformSubtypeDeleteResponse(const Datastruct::DetectPlatformSubtypeDeleteResponse &)));

		connect(SignalDispatch::instance(), SIGNAL(respDetectPlatformSubtypeModifyResponse(const Datastruct::DetectPlatformSubtypeModifyResponse &)),
			this, SLOT(processDetectPlatformSubtypeModifyResponse(const Datastruct::DetectPlatformSubtypeModifyResponse &)));
	}

	void DetectPlatformPage::refreshCurrDetectPlatform()
	{
		Datastruct::LoadAllDetectPlatformsRequest request;
		DataNetConnector::instance()->write(request);
	}

	void DetectPlatformPage::insertDetectPlatform(int id, QString name)
	{
		Datastruct::DetectPlatformCreateRequest request;
		request.m_id = id;
		request.m_name = name;
		DataNetConnector::instance()->write(request);
	}

	void DetectPlatformPage::deleteDetectPlatform(int id)
	{
		Datastruct::DetectPlatformDeleteRequest request;
		request.m_id = id;
		DataNetConnector::instance()->write(request);
	}

	void DetectPlatformPage::modifyDetectPlatform(Datastruct::DetectPlatformEntityData data)
	{
		Datastruct::DetectPlatformModifyRequest request;
		request.m_id = data.id;
		request.m_name = data.name;
		DataNetConnector::instance()->write(request);
	}

	void DetectPlatformPage::refreshCurrDetectPlatformSubtype(int detectId)
	{
		Datastruct::LoadAllDetectPlatformSubtypesRequest request;
		request.m_detectId = detectId;
		DataNetConnector::instance()->write(request);
	}

	void DetectPlatformPage::insertDetectPlatformSubtype(int detectId,  int id, QString name)
	{
		Datastruct::DetectPlatformSubtypeCreateRequest request;
		request.m_detectId = id;
		request.m_id = id;
		request.m_name = name;
		DataNetConnector::instance()->write(request);
	}

	void DetectPlatformPage::deleteDetectPlatformSubtype(Datastruct::DetectPlatformSubtypeEntityData data)
	{
		Datastruct::DetectPlatformSubtypeModifyRequest request;
		request.m_id = data.id;
		request.m_detectId = data.detectId;
		DataNetConnector::instance()->write(request);
	}

	void DetectPlatformPage::modifyDetectPlatformSubtype(Datastruct::DetectPlatformSubtypeEntityData data)
	{
		Datastruct::DetectPlatformSubtypeModifyRequest request;
		request.m_id = data.id;
		request.m_detectId = data.detectId;
		request.m_name = data.name;
		DataNetConnector::instance()->write(request);
	}

}//namespace Related 