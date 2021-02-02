#include "detectplatformeditdialog.h"

#include <QDebug>

#include "../../../net/datanetconnector.h"
#include "../../../net/signaldispatch.h"

namespace Related {

	DetectPlatformEditDialog::DetectPlatformEditDialog(QWidget *parent)
		: Base::DialogProxy(parent)
	{
		setButton(DialogProxy::Ok, this, SLOT(acceptOk()));
		setButton(DialogProxy::Cancel, this, SLOT(reject()));

		init();
		initConnect();
	}

	DetectPlatformEditDialog::~DetectPlatformEditDialog()
	{
	}
	void DetectPlatformEditDialog::acceptOk()
	{
		QString name = m_nameLineEdit->text();
		createNewDetectPlatform(name);
	}

	void DetectPlatformEditDialog::processDetectPlatformCreateResponse(const Datastruct::DetectPlatformCreateResponse & response)
	{
		if (response.m_createResult) {

			this->accept();
		}
		else
		{
			int result = Base::RMessageBox::information(this,
				QStringLiteral("提示"), QStringLiteral("创建数据失败，请从新填写信息。"),
				Base::RMessageBox::Yes | Base::RMessageBox::No);
			if (result != Base::RMessageBox::Yes) {
				this->reject();
			}
		}
	}

	void DetectPlatformEditDialog::init()
	{
		QLabel * nameLabel = new QLabel();
		nameLabel->setText(QStringLiteral("平台名:"));
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
	}

	void DetectPlatformEditDialog::createNewDetectPlatform(QString name)
	{
		Datastruct::DetectPlatformCreateRequest request;
		request.m_name = name;
		DataNetConnector::instance()->write(request);
	}

}//namespace Related 