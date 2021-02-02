/*!
 * @brief     试验记录编辑窗口  
 * @details   该界面用于创建与修改试验记录数据信息
 * @author    yzg
 * @version   1.0
 * @date      2021.01.29 14:48:14
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
#include <base/util/rutil.h>
#include <commondefines/protocol.h>

#include "../../datastruct.h"

namespace Related {

	class ExperimentRecordEditDialog : public Base::DialogProxy
	{
		Q_OBJECT

	public:
		ExperimentRecordEditDialog(QWidget *parent = nullptr);
		~ExperimentRecordEditDialog();

		enum ExperimentRecordOperatioType {
			ERD_Create,					/*!< 创建  */
			ERD_Modify,					/*!< 修改 */
		};

		void setExperimentRecordDataOperatioType(ExperimentRecordOperatioType type);

		void setTaskId(QString taskId);

		void setExperimentRecordEntityData(Datastruct::ExperimentRecordEntityData data);

	private slots:
		void acceptOk();
		void processExperimentRecordCreateResponse(const Datastruct::ExperimentRecordCreateResponse & response);
		void processExperimentRecordModifyResponse(const Datastruct::ExperimentRecordModifyResponse & response);
	
	private:
		void init();
		void initConnect();

		void createNewExperimentRecord(Datastruct::ExperimentRecordCreateRequest request);
		void modifyExperimentRecord(Datastruct::ExperimentRecordEntityData data);
	private:
		QLineEdit * m_lonLineEdit;
		QLineEdit * m_latLineEdit;
		QLineEdit * m_setHeadingDegreeLineEdit;
		QLineEdit * m_actualHeadingDegreeLineEdit;


		ExperimentRecordOperatioType m_operatioType;
		QString m_taskId;
		Datastruct::ExperimentRecordEntityData m_experimentRecordData;
	};

}//namespace Related 