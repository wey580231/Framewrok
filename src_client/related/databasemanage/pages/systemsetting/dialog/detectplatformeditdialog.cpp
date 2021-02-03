#include "detectplatformeditdialog.h"

#include <QDebug>

#include "../../../net/datanetconnector.h"
#include "../../../net/signaldispatch.h"

namespace Related {

	DetectPlatformEditDialog::DetectPlatformEditDialog(QWidget *parent)
		: Base::DialogProxy(parent),
		m_nameLineEdit(nullptr),
		m_platformId(0)
	{
		setButton(DialogProxy::Ok, this, SLOT(acceptOk()));
		setButton(DialogProxy::Cancel, this, SLOT(reject()));
	}

	DetectPlatformEditDialog::~DetectPlatformEditDialog()
	{
	}

	void DetectPlatformEditDialog::setEditType(EditType type)
	{
		m_editType = type;

		if (m_editType == E_DetectPlatform) {
			setTitle(QStringLiteral("创建侦查平台"));
		}
		else if(m_editType == E_DetectPlatformSubtype) {
			setTitle(QStringLiteral("创建平台亚型"));
		}

		init();
		initConnect();
	}

	void DetectPlatformEditDialog::setPlatformId(int platformId)
	{
		m_platformId = platformId;
	}

	void DetectPlatformEditDialog::acceptOk()
	{
		QString name = m_nameLineEdit->text();
		if (m_editType == E_DetectPlatform){
			createNewDetectPlatform(name);
		}
		else if (m_editType == E_DetectPlatformSubtype) {
			createNewDetectPlatformSubtype(name, m_platformId);
		}
	}

	void DetectPlatformEditDialog::processDetectPlatformCreateResponse(const Datastruct::DetectPlatformCreateResponse & response)
	{
		if (response.m_createResult) {
			this->accept();
		}
		else {
			int result = Base::RMessageBox::information(this,
				QStringLiteral("提示"), QStringLiteral("增加侦查平台失败，请从新填写信息。"),
				Base::RMessageBox::Yes | Base::RMessageBox::No);
			if (result != Base::RMessageBox::Yes) {
				this->reject();
			}
		}
	}

	void DetectPlatformEditDialog::processDetectPlatformSubtypeCreateResponse(const Datastruct::DetectPlatformSubtypeCreateResponse & response)
	{
		if (response.m_createResult) {
			this->accept();
		}
		else {
			int result = Base::RMessageBox::information(this,
				QStringLiteral("提示"), QStringLiteral("增加平台亚型失败，请从新填写信息。"),
				Base::RMessageBox::Yes | Base::RMessageBox::No);
			if (result != Base::RMessageBox::Yes) {
				this->reject();
			}
		}
	}

	void DetectPlatformEditDialog::init()
	{
		QLabel * nameLabel = new QLabel();
		if (m_editType == E_DetectPlatform) {
			nameLabel->setText(QStringLiteral("平台名:"));
		}  
		else if (m_editType == E_DetectPlatformSubtype) {
			nameLabel->setText(QStringLiteral("平台亚型名:"));
		}

		m_nameLineEdit = new QLineEdit();

		QWidget * mainWidget = new QWidget();
		QHBoxLayout * mianLayout = new QHBoxLayout();
		mianLayout->addWidget(nameLabel);
		mianLayout->addWidget(m_nameLineEdit);

		mianLayout->setContentsMargins(4,4,4,4);
		mainWidget->setLayout(mianLayout);
		this->setContentWidget(mainWidget);
	}

	void DetectPlatformEditDialog::initConnect()
	{
		connect(SignalDispatch::instance(), SIGNAL(respDetectPlatformCreateResponse(const Datastruct::DetectPlatformCreateResponse &)),
			this, SLOT(processDetectPlatformCreateResponse(const Datastruct::DetectPlatformCreateResponse &)));

		connect(SignalDispatch::instance(), SIGNAL(respDetectPlatformSubtypeCreateResponse(const Datastruct::DetectPlatformSubtypeCreateResponse &)),
			this, SLOT(processDetectPlatformSubtypeCreateResponse(const Datastruct::DetectPlatformSubtypeCreateResponse &)));
	}

	void DetectPlatformEditDialog::createNewDetectPlatform(QString name)
	{
		Datastruct::DetectPlatformCreateRequest request;
		request.m_name = name;
		DataNetConnector::instance()->write(request);
	}

	void DetectPlatformEditDialog::createNewDetectPlatformSubtype(QString name, int platformId)
	{
		Datastruct::DetectPlatformSubtypeCreateRequest request;
		request.m_detectId = platformId;
		request.m_name = name;
		DataNetConnector::instance()->write(request);
	}

}//namespace Related 