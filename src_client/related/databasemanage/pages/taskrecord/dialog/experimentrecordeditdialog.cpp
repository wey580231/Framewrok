#include "experimentrecordeditdialog.h"

#include <QDebug>

#include "../../../net/datanetconnector.h"
#include "../../../net/signaldispatch.h"

namespace Related {

	ExperimentRecordEditDialog::ExperimentRecordEditDialog(QString taskId, ExperimentRecordOperatioType type, QWidget *parent)
		: Base::DialogProxy(parent),
		m_taskId(taskId), m_operatioType(type)
	{
		if (m_operatioType == ERD_Create) {
			setTitle(QStringLiteral("创建试验记录"));
		}
		else if (m_operatioType == ERD_Modify) {
			setTitle(QStringLiteral("修改试验记录"));
		}

		setButton(DialogProxy::Ok, this, SLOT(acceptOk()));
		setButton(DialogProxy::Cancel, this, SLOT(reject()));

		this->setMinimumSize(500, 420);

		init();
		initConnect();
	}

	ExperimentRecordEditDialog::~ExperimentRecordEditDialog()
	{

	}

	void ExperimentRecordEditDialog::setExperimentRecordEntityData(Datastruct::ExperimentRecordEntityData data)
	{
		m_experimentRecordData = data;

		m_detectPlatformComboBox->setCurrentText(data.platformId);

		m_lonEdit->setValue(data.lon);
		m_latEdit->setValue(data.lat);
		m_setHeadingDegreeLineEdit->setText(QString::number(data.setHeadingDegree));
		m_actualHeadingDegreeLineEdit->setText(QString::number(data.actualHeadingDegree));
	}

	void ExperimentRecordEditDialog::acceptOk()
	{
		if (m_operatioType == ERD_Create) 
		{
			QDateTime current_date_time = QDateTime::currentDateTime();
			Datastruct::ExperimentRecordCreateRequest request;
			request.m_platformId = m_detectPlatformComboBox->currentText();
			request.m_floatingTime = current_date_time.toString(TIME_FORMAT);
			request.m_lon = m_lonEdit->getValue();
			request.m_lat = m_latEdit->getValue();
			request.m_setHeadingDegree = m_setHeadingDegreeLineEdit->text().toDouble();
			request.m_actualHeadingDegree = m_actualHeadingDegreeLineEdit->text().toDouble();
			request.m_acousticState = 0;
			request.m_targetNum = 1;
			request.m_underwaterTargetNum = 1;
			request.m_underwaterTargetInfo = QStringLiteral("0101");
			request.m_maxDepth = 0;
			request.m_profileIndex = 1;
			request.m_profileLength = 1;
			request.m_profileDistance = 1;

			createNewExperimentRecord(request);
		}
		else if(m_operatioType == ERD_Modify)
		{
			m_experimentRecordData.lon = m_lonEdit->getValue();
			m_experimentRecordData.lat = m_latEdit->getValue();
			m_experimentRecordData.setHeadingDegree = m_setHeadingDegreeLineEdit->text().toDouble();
			m_experimentRecordData.actualHeadingDegree = m_actualHeadingDegreeLineEdit->text().toDouble();

			modifyExperimentRecord(m_experimentRecordData);
		}
	}

	void ExperimentRecordEditDialog::processExperimentRecordCreateResponse(const Datastruct::ExperimentRecordCreateResponse & response)
	{
		if (response.m_createResult == true) {
			this->accept();
		} 
		else
		{
			int result = Base::RMessageBox::information(this, 
						QStringLiteral("提示"), QStringLiteral("创建数据失败，请从新填写信息。"), 
						Base::RMessageBox::Yes | Base::RMessageBox::No);
			if (result != Base::RMessageBox::Yes) 
			{
				this->reject();
			}
		}
	}

	void ExperimentRecordEditDialog::processExperimentRecordModifyResponse(const Datastruct::ExperimentRecordModifyResponse & response)
	{
		if (response.m_modifyResult) {
			this->accept();
		}
		else
		{
			int result = Base::RMessageBox::information(this, 
						QStringLiteral("提示"), QStringLiteral("修改数据失败，请从新填写信息。"),
						Base::RMessageBox::Yes | Base::RMessageBox::No);
			if (result != Base::RMessageBox::Yes) {
				this->reject();
			}
		}
	}

	void ExperimentRecordEditDialog::init()
	{
		QWidget *mainWidget = new QWidget();

		QLabel * detectPlatformLabel = new QLabel();
		detectPlatformLabel->setText(QStringLiteral("侦查平台:"));
		detectPlatformLabel->setAlignment(Qt::AlignRight);
		m_detectPlatformComboBox = new QComboBox();
		QStringList strList;
		strList << QStringLiteral("HXJ01")
			<< QStringLiteral("HXJ02")
			<< QStringLiteral("HXJ03")
			<< QStringLiteral("HXJ04");
		m_detectPlatformComboBox->addItems(strList);

		QLabel * lonLabel = new QLabel();
		lonLabel->setText(QStringLiteral("上浮经度:"));
		lonLabel->setAlignment(Qt::AlignRight);
		m_lonEdit = new Base::RLonLatEdit(true);

		QLabel * latLabel = new QLabel();
		latLabel->setText(QStringLiteral("上浮纬度:"));
		latLabel->setAlignment(Qt::AlignRight);
		m_latEdit = new Base::RLonLatEdit(false);

		QLabel * setHeadingDegreeLabel = new QLabel();
		setHeadingDegreeLabel->setText(QStringLiteral("设置航向角:"));
		setHeadingDegreeLabel->setAlignment(Qt::AlignRight);
		m_setHeadingDegreeLineEdit = new QLineEdit();
		m_setHeadingDegreeLineEdit->setText(QString::number(0));

		QLabel * actualHeadingDegreeLabel = new QLabel();
		actualHeadingDegreeLabel->setText(QStringLiteral("实际航向角:"));
		actualHeadingDegreeLabel->setAlignment(Qt::AlignRight);
		m_actualHeadingDegreeLineEdit = new QLineEdit();
		m_actualHeadingDegreeLineEdit->setText(QString::number(0));

		QGridLayout * mainLayout = new QGridLayout();
		mainLayout->addWidget(detectPlatformLabel, 0, 0);
		mainLayout->addWidget(m_detectPlatformComboBox, 0, 1);

		mainLayout->addWidget(lonLabel, 1, 0);
		mainLayout->addWidget(m_lonEdit, 1, 1);
		mainLayout->addWidget(latLabel, 2, 0);
		mainLayout->addWidget(m_latEdit, 2, 1);
		mainLayout->addWidget(setHeadingDegreeLabel, 3, 0);
		mainLayout->addWidget(m_setHeadingDegreeLineEdit, 3, 1);
		mainLayout->addWidget(actualHeadingDegreeLabel, 4, 0);
		mainLayout->addWidget(m_actualHeadingDegreeLineEdit, 4, 1);
		mainLayout->setContentsMargins(100, 4, 100, 4);
		mainWidget->setLayout(mainLayout);

		this->setContentWidget(mainWidget);
	}

	void ExperimentRecordEditDialog::initConnect()
	{
		connect(SignalDispatch::instance(), SIGNAL(respExperimentRecordCreateResponse(const Datastruct::ExperimentRecordCreateResponse &)),
			this, SLOT(processExperimentRecordCreateResponse(const Datastruct::ExperimentRecordCreateResponse &)));

		connect(SignalDispatch::instance(), SIGNAL(respExperimentRecordModifyResponse(const Datastruct::ExperimentRecordModifyResponse &)),
			this, SLOT(processExperimentRecordModifyResponse(const Datastruct::ExperimentRecordModifyResponse &)));

	}
	void ExperimentRecordEditDialog::createNewExperimentRecord(Datastruct::ExperimentRecordCreateRequest request)
	{
		request.m_id = Base::RUtil::UUID();
		request.m_taskId = m_taskId;
		DataNetConnector::instance()->write(request);
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

		DataNetConnector::instance()->write(request);
	}
}//namespace Related 