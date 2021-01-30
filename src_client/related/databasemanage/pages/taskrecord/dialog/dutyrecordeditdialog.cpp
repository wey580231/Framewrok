#include "dutyrecordeditdialog.h"

#include <QDebug>

#include "../../../net/datanetconnector.h"
#include "../../../net/signaldispatch.h"

namespace Related {

	DutyRecordEditDialog::DutyRecordEditDialog(QWidget *parent)
		: Base::DialogProxy(parent)
	{
		setTitle(QStringLiteral("值班日志编辑"));
		setButton(DialogProxy::Ok, this, SLOT(acceptOk()));
		setButton(DialogProxy::Cancel, this, SLOT(reject()));

		init();
		initConnect();
	}

	DutyRecordEditDialog::~DutyRecordEditDialog()
	{
	}

	void DutyRecordEditDialog::setDutyRecordEntityData(Datastruct::DutyRecordEntityData data)
	{
		m_dutyRecordEntitydata = data;

		m_windLineEdit->setText(QString::number(data.wind));
		m_windSpeedLineEdit->setText(QString::number(data.windSpeed));
		m_waveHighLineEdit->setText(QString::number(data.waveHigh));
		m_oceanCurrentsLineEdit->setText(QString::number(data.oceanCurrents));
	}

	void DutyRecordEditDialog::acceptOk()
	{
		m_dutyRecordEntitydata.wind = m_windLineEdit->text().toDouble();
		m_dutyRecordEntitydata.windSpeed = m_windSpeedLineEdit->text().toDouble();
		m_dutyRecordEntitydata.waveHigh = m_waveHighLineEdit->text().toDouble();
		m_dutyRecordEntitydata.oceanCurrents = m_oceanCurrentsLineEdit->text().toDouble();
		modifyDutyRecord(m_dutyRecordEntitydata);
	}

	void DutyRecordEditDialog::processDutyRecordModifyResponse(const Datastruct::DutyRecordModifyResponse & response)
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

	void DutyRecordEditDialog::init()
	{
		QWidget *mainWidget = new QWidget();

		QLabel * windLabel = new QLabel();
		windLabel->setText(QStringLiteral("风向:"));
		m_windLineEdit = new QLineEdit();

		QLabel * windSpeedLabel = new QLabel();
		windSpeedLabel->setText(QStringLiteral("风速:"));
		m_windSpeedLineEdit = new QLineEdit();

		QLabel * waveHighLabel = new QLabel();
		waveHighLabel->setText(QStringLiteral("浪高:"));
		m_waveHighLineEdit = new QLineEdit();

		QLabel * oceanCurrentsLabel = new QLabel();
		oceanCurrentsLabel->setText(QStringLiteral("洋流:"));
		m_oceanCurrentsLineEdit = new QLineEdit();

		QGridLayout * mainLayout = new QGridLayout();
		mainLayout->addWidget(windLabel, 0, 0);
		mainLayout->addWidget(m_windLineEdit, 0, 1);
		mainLayout->addWidget(windSpeedLabel, 1, 0);
		mainLayout->addWidget(m_windSpeedLineEdit, 1, 1);
		mainLayout->addWidget(waveHighLabel, 2, 0);
		mainLayout->addWidget(m_waveHighLineEdit, 2, 1);
		mainLayout->addWidget(oceanCurrentsLabel, 3, 0);
		mainLayout->addWidget(m_oceanCurrentsLineEdit, 3, 1);
		mainLayout->setContentsMargins(4, 4, 4, 4);
		mainWidget->setLayout(mainLayout);
		this->setContentWidget(mainWidget);
	}

	void DutyRecordEditDialog::initConnect()
	{
		connect(SignalDispatch::instance(), SIGNAL(respDutyRecordModifyResponse(const Datastruct::DutyRecordModifyResponse &)),
			this, SLOT(processDutyRecordModifyResponse(const Datastruct::DutyRecordModifyResponse &)));

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