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
#include <QDateTimeEdit>
#include <QDateTime>
#include <QPlainTextEdit>

#include <base/selfwidget/dialogproxy.h>
#include <base/selfwidget/rmessagebox.h>
#include <base/util/rutil.h>

#include <commondefines/protocol.h>

#include "../../datastruct.h"

namespace Related {

	class DutyRecordEditDialog : public Base::DialogProxy
	{
		Q_OBJECT

	public:
		DutyRecordEditDialog(QWidget *parent = Q_NULLPTR);
		~DutyRecordEditDialog();

		enum DutyRecordOperatioType {
			DRD_Create,					/*!< 创建  */
			DRD_Modify,					/*!< 修改 */
		};

		void setDutyRecordDataOperatioType(DutyRecordOperatioType type);

		void setTaskId(QString taskId);

		void setDutyRecordEntityData(Datastruct::DutyRecordEntityData data);

	private slots:
		void acceptOk();
		void processDutyRecordCreateResponse(const Datastruct::DutyRecordCreateResponse & response);
		void processDutyRecordModifyResponse(const Datastruct::DutyRecordModifyResponse & response);

	private:
		void init();
		void initConnect();
		void createNewDutyRecord(Datastruct::DutyRecordCreateRequest request);
		void modifyDutyRecord(Datastruct::DutyRecordEntityData info);

	private:
		QDateTimeEdit * m_createDateTimeEdit;
		QLineEdit * m_seaConditionLineEdit;
		QLineEdit * m_windLineEdit;					
		QLineEdit * m_windSpeedLineEdit;
		QLineEdit * m_waveHighLineEdit;
		QLineEdit * m_oceanCurrentsLineEdit;
		QPlainTextEdit * m_descriptionPlainTextEdit;

		DutyRecordOperatioType m_operatioType;						/*!< 控制类型 */
		QString m_taskId;											/*!< 任务Id */

		Datastruct::DutyRecordEntityData m_dutyRecordEntitydata;	/*!<  */
	};

} //namespace Related