#include "dutyrecordeditdialog.h"

#include <QDebug>

#include "../../../net/datanetconnector.h"
#include "../../../net/signaldispatch.h"

namespace Related {

	DutyRecordEditDialog::DutyRecordEditDialog(QWidget *parent)
		: Base::DialogProxy(parent)
	{
		setButton(DialogProxy::Ok, this, SLOT(acceptOk()));
		setButton(DialogProxy::Cancel, this, SLOT(reject()));

		this->setMinimumSize(500, 420);

		init();
		initConnect();
	}

	DutyRecordEditDialog::~DutyRecordEditDialog()
	{
	}

	void DutyRecordEditDialog::setDutyRecordDataOperatioType(DutyRecordOperatioType type)
	{
		m_operatioType = type;
		if (m_operatioType == DRD_Create) 
		{
			setTitle(QStringLiteral("新建值班日志"));
		}
		else if (m_operatioType == DRD_Modify)
		{
			setTitle(QStringLiteral("修改值班日志辑"));
		}
	}

	void DutyRecordEditDialog::setTaskId(QString taskId)
	{
		m_taskId = taskId;
	}

	void DutyRecordEditDialog::setDutyRecordEntityData(Datastruct::DutyRecordEntityData data)
	{
		m_dutyRecordEntitydata = data;

		QDateTime createTime = QDateTime::fromString(data.createTime, "yyyy-MM-dd hh:mm:ss");
		m_createDateTimeEdit->setDateTime(createTime);
		m_seaConditionLineEdit->setText(data.seaCondition);
		m_windLineEdit->setText(QString::number(data.wind));
		m_windSpeedLineEdit->setText(QString::number(data.windSpeed));
		m_waveHighLineEdit->setText(QString::number(data.waveHigh));
		m_oceanCurrentsLineEdit->setText(QString::number(data.oceanCurrents));
		m_descriptionLineEdit->setText(data.description);
	}

	void DutyRecordEditDialog::acceptOk()
	{
		if (m_operatioType == DRD_Create)
		{
			Datastruct::DutyRecordCreateRequest request;
			request.m_createTime = m_createDateTimeEdit->dateTime().toString("yyyy-MM-dd hh:mm:ss");
			request.m_seaCondition = m_seaConditionLineEdit->text();
			request.m_wind = m_windLineEdit->text().toDouble();
			request.m_windSpeed = m_windSpeedLineEdit->text().toDouble();
			request.m_waveHigh = m_waveHighLineEdit->text().toDouble();
			request.m_oceanCurrents = m_oceanCurrentsLineEdit->text().toDouble();
			request.m_description = m_descriptionLineEdit->text();

			createNewDutyRecord(request);
		}
		else if(m_operatioType == DRD_Modify)
		{
			m_dutyRecordEntitydata.wind = m_windLineEdit->text().toDouble();
			m_dutyRecordEntitydata.windSpeed = m_windSpeedLineEdit->text().toDouble();
			m_dutyRecordEntitydata.waveHigh = m_waveHighLineEdit->text().toDouble();
			m_dutyRecordEntitydata.oceanCurrents = m_oceanCurrentsLineEdit->text().toDouble();
			modifyDutyRecord(m_dutyRecordEntitydata);
		}
	}

	void DutyRecordEditDialog::processDutyRecordCreateResponse(const Datastruct::DutyRecordCreateResponse & response)
	{
		if (response.m_createResult == true) {

			this->accept();
		}
		else
		{
			int result = Base::RMessageBox::information(this, 
						QStringLiteral("提示"),  QStringLiteral("创建数据失败，请从新填写信息。"),
						Base::RMessageBox::Yes | Base::RMessageBox::No);
			if (result != Base::RMessageBox::Yes) {
				this->reject();
			}
		}
	}

	void DutyRecordEditDialog::processDutyRecordModifyResponse(const Datastruct::DutyRecordModifyResponse & response)
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

	void DutyRecordEditDialog::init()
	{
		QWidget *mainWidget = new QWidget();

		QLabel * createTime = new QLabel();
		createTime->setText(QStringLiteral("时间:"));

		m_createDateTimeEdit = new QDateTimeEdit();
		m_createDateTimeEdit->setDisplayFormat("yyyy-MM-dd HH:mm:ss");
		QDateTime current_date_time = QDateTime::currentDateTime();
		m_createDateTimeEdit->setDateTime(current_date_time);

		QLabel * seaConditionLabel = new QLabel();
		seaConditionLabel->setText(QStringLiteral("海况:"));
		m_seaConditionLineEdit = new QLineEdit();

		QLabel * windLabel = new QLabel();
		windLabel->setText(QStringLiteral("风向:"));

		m_windLineEdit = new QLineEdit();
		m_windLineEdit->setText(QString::number(0));

		QLabel * windSpeedLabel = new QLabel();
		windSpeedLabel->setText(QStringLiteral("风速:"));
		m_windSpeedLineEdit = new QLineEdit();
		m_windSpeedLineEdit->setText(QString::number(0));

		QLabel * waveHighLabel = new QLabel();
		waveHighLabel->setText(QStringLiteral("浪高:"));
		m_waveHighLineEdit = new QLineEdit();
		m_waveHighLineEdit->setText(QString::number(0));

		QLabel * oceanCurrentsLabel = new QLabel();
		oceanCurrentsLabel->setText(QStringLiteral("洋流:"));
		m_oceanCurrentsLineEdit = new QLineEdit();
		m_oceanCurrentsLineEdit->setText(QString::number(0));

		QLabel * descriptionLabel = new QLabel();
		descriptionLabel->setText(QStringLiteral("描述:"));
		m_descriptionLineEdit = new QLineEdit();

		QGridLayout * mainLayout = new QGridLayout();
		mainLayout->addWidget(createTime, 0, 0);
		mainLayout->addWidget(m_createDateTimeEdit, 0, 1);

		mainLayout->addWidget(seaConditionLabel, 0, 2);
		mainLayout->addWidget(m_seaConditionLineEdit, 0, 3);

		mainLayout->addWidget(windLabel, 1, 0);
		mainLayout->addWidget(m_windLineEdit, 1, 1);
		mainLayout->addWidget(windSpeedLabel, 1, 2);
		mainLayout->addWidget(m_windSpeedLineEdit, 1, 3);
		mainLayout->addWidget(waveHighLabel, 2, 0);
		mainLayout->addWidget(m_waveHighLineEdit, 2, 1);
		mainLayout->addWidget(oceanCurrentsLabel, 2, 2);
		mainLayout->addWidget(m_oceanCurrentsLineEdit, 2, 3);

		mainLayout->addWidget(descriptionLabel, 3, 0);
		mainLayout->addWidget(m_descriptionLineEdit, 3, 1, 1, 3);

		mainLayout->setSpacing(20);
		mainLayout->setContentsMargins(30, 4, 30, 4);
		mainWidget->setLayout(mainLayout);
		this->setContentWidget(mainWidget);
	}

	void DutyRecordEditDialog::initConnect()
	{
		connect(SignalDispatch::instance(), SIGNAL(respDutyRecordCreateResponse(const Datastruct::DutyRecordCreateResponse &)),
			this, SLOT(processDutyRecordCreateResponse(const Datastruct::DutyRecordCreateResponse &)));

		connect(SignalDispatch::instance(), SIGNAL(respDutyRecordModifyResponse(const Datastruct::DutyRecordModifyResponse &)),
			this, SLOT(processDutyRecordModifyResponse(const Datastruct::DutyRecordModifyResponse &)));
	}

	void DutyRecordEditDialog::createNewDutyRecord(Datastruct::DutyRecordCreateRequest request)
	{
		request.m_id = Base::RUtil::UUID();
		request.m_taskId = m_taskId;
		DataNetConnector::instance()->write(request);
	}

	void DutyRecordEditDialog::modifyDutyRecord(Datastruct::DutyRecordEntityData info)
	{
		Datastruct::DutyRecordModifyRequest request;
		request.m_id = info.id;
		request.m_taskId = info.taskId;
		request.m_createTime = info.createTime;
		request.m_description = info.description;
		request.m_seaCondition = info.seaCondition;
		request.m_wind = info.wind;
		request.m_windSpeed = info.windSpeed;
		request.m_waveHigh = info.waveHigh;
		request.m_oceanCurrents = info.oceanCurrents;
		DataNetConnector::instance()->write(request);
	}

}//namespace Related 