/*!
 * @brief    值班日志编辑窗口 
 * @details 
 * @author    yzg
 * @version   1.0
 * @date      2021.01.29 14:44:11
 * @warning
 * @copyright NanJing RenGu.
 * @note
 */
#pragma once

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>

#include <base/selfwidget/dialogproxy.h>
#include <base/selfwidget/rmessagebox.h>

#include <commondefines/protocol.h>

namespace Related {

	class DutyRecordEditDialog : public Base::DialogProxy
	{
		Q_OBJECT

	public:
		DutyRecordEditDialog(QWidget *parent = Q_NULLPTR);
		~DutyRecordEditDialog();

		void setDutyRecordEntityData(Datastruct::DutyRecordEntityData data);

	private slots:
		void acceptOk();
		void processDutyRecordModifyResponse(const Datastruct::DutyRecordModifyResponse & response);

	private:
		void init();
		void initConnect();
		void modifyDutyRecord(Datastruct::DutyRecordEntityData info);
	private:
		QLineEdit * m_windLineEdit;
		QLineEdit * m_windSpeedLineEdit;
		QLineEdit * m_waveHighLineEdit;
		QLineEdit * m_oceanCurrentsLineEdit;

		Datastruct::DutyRecordEntityData m_dutyRecordEntitydata;
	};

} //namespace Related