#include "experimentrecordeditdialog.h"

#include <QDebug>

#include "../../../net/netconnector.h"
#include "../../../net/signaldispatch.h"

namespace Related {

	ExperimentRecordEditDialog::ExperimentRecordEditDialog(QWidget *parent)
		: Base::DialogProxy(parent)
	{
		setTitle(QStringLiteral("试验记录编辑"));
		setButton(DialogProxy::Ok, this, SLOT(acceptOk()));
		setButton(DialogProxy::Cancel, this, SLOT(reject()));

		init();
		initConnect();
	}

	ExperimentRecordEditDialog::~ExperimentRecordEditDialog()
	{

	}

	void ExperimentRecordEditDialog::setExperimentRecordEntityData(Datastruct::ExperimentRecordEntityData data)
	{
		m_experimentRecordData = data;

		m_lonLineEdit->setText(QString::number(data.lon));
		m_latLineEdit->setText(QString::number(data.lat));
		m_setHeadingDegreeLineEdit->setText(QString::number(data.setHeadingDegree));
		m_actualHeadingDegreeLineEdit->setText(QString::number(data.actualHeadingDegree));

	}

	void ExperimentRecordEditDialog::acceptOk()
	{
		m_experimentRecordData.lon = m_lonLineEdit->text().toDouble();
		m_experimentRecordData.lat = m_latLineEdit->text().toDouble();
		m_experimentRecordData.setHeadingDegree = m_setHeadingDegreeLineEdit->text().toDouble();
		m_experimentRecordData.actualHeadingDegree = m_actualHeadingDegreeLineEdit->text().toDouble();

		modifyExperimentRecord(m_experimentRecordData);
	}

	void ExperimentRecordEditDialog::processExperimentRecordModifyResponse(const Datastruct::ExperimentRecordModifyResponse & response)
	{
		if (response.m_modifyResult) {
			this->accept();
		}
		else
		{
			int result = Base::RMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("修改数据失败，请从新填写信息。"), Base::RMessageBox::Yes | Base::RMessageBox::No);
			if (result != Base::RMessageBox::Yes) {
				this->reject();
			}
		}
	}

	void ExperimentRecordEditDialog::init()
	{
		QWidget *mainWidget = new QWidget();

		QLabel * lonLabel = new QLabel();
		lonLabel->setText(QStringLiteral("上浮经度:"));
		m_lonLineEdit = new QLineEdit();

		QLabel * latLabel = new QLabel();
		latLabel->setText(QStringLiteral("上浮纬度:"));
		m_latLineEdit = new QLineEdit();

		QLabel * setHeadingDegreeLabel = new QLabel();
		setHeadingDegreeLabel->setText(QStringLiteral("设置航向角:"));
		m_setHeadingDegreeLineEdit = new QLineEdit();

		QLabel * actualHeadingDegreeLabel = new QLabel();
		actualHeadingDegreeLabel->setText(QStringLiteral("实际航向角:"));
		m_actualHeadingDegreeLineEdit = new QLineEdit();

		QGridLayout * mainLayout = new QGridLayout();
		mainLayout->addWidget(lonLabel, 0, 0);
		mainLayout->addWidget(m_lonLineEdit, 0, 1);
		mainLayout->addWidget(latLabel, 1, 0);
		mainLayout->addWidget(m_latLineEdit, 1, 1);
		mainLayout->addWidget(setHeadingDegreeLabel, 2, 0);
		mainLayout->addWidget(m_setHeadingDegreeLineEdit, 2, 1);
		mainLayout->addWidget(actualHeadingDegreeLabel, 3, 0);
		mainLayout->addWidget(m_actualHeadingDegreeLineEdit, 3, 1);
		mainLayout->setContentsMargins(4, 4, 4, 4);
		mainWidget->setLayout(mainLayout);
		this->setContentWidget(mainWidget);
	}

	void ExperimentRecordEditDialog::initConnect()
	{
		connect(SignalDispatch::instance(), SIGNAL(respExperimentRecordModifyResponse(const Datastruct::ExperimentRecordModifyResponse &)),
			this, SLOT(processExperimentRecordModifyResponse(const Datastruct::ExperimentRecordModifyResponse &)));

	}
	void ExperimentRecordEditDialog::modifyExperimentRecord(Datastruct::ExperimentRecordEntityData data)
	{
		Datastruct::ExperimentRecordModifyRequest request;
		request.m_id = data.id;
		request.m_taskId = data.taskId;
		request.m_platformId = data.platformId;
		request.m_floatingTime = data.floatingTime;
		request.m_lon = data.lon;
		request.m_lat = data.lat;
		request.m_setHeadingDegree = data.setHeadingDegree;
		request.m_actualHeadingDegree = data.actualHeadingDegree;
		request.m_acousticState = data.acousticState;
		request.m_targetNum = data.targetNum;
		request.m_underwaterTargetNum = data.underwaterTargetNum;
		request.m_underwaterTargetInfo = data.underwaterTargetInfo;
		request.m_maxDepth = data.maxDepth;
		request.m_profileIndex = data.profileIndex;
		request.m_profileLength = data.profileLength;
		request.m_profileDistance = data.profileDistance;

		NetConnector::instance()->write(request);
	}
}//namespace Related 