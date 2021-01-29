/*!
 * @brief     ÊÔÑé¼ÇÂ¼±à¼­´°¿Ú  
 * @details 
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

#include <commondefines/protocol.h>

namespace Related {

	class ExperimentRecordEditDialog : public Base::DialogProxy
	{
		Q_OBJECT

	public:
		ExperimentRecordEditDialog(QWidget *parent = nullptr);
		~ExperimentRecordEditDialog();

		void setExperimentRecordEntityData(Datastruct::ExperimentRecordEntityData data);

	private slots:
		void acceptOk();
		void processExperimentRecordModifyResponse(const Datastruct::ExperimentRecordModifyResponse & response);
	
	private:
		void init();
		void initConnect();
		void modifyExperimentRecord(Datastruct::ExperimentRecordEntityData data);
	private:
		QLineEdit * m_lonLineEdit;
		QLineEdit * m_latLineEdit;
		QLineEdit * m_setHeadingDegreeLineEdit;
		QLineEdit * m_actualHeadingDegreeLineEdit;


		Datastruct::ExperimentRecordEntityData m_experimentRecordData;
	};

}//namespace Related 