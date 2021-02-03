/*!
 * @brief     侦查平台/侦查平台亚型编辑子界面
 * @details 
 * @author    yzg
 * @version   1.0
 * @date      2021.02.02 17:10:20
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

	class DetectPlatformEditDialog : public Base::DialogProxy
	{
		Q_OBJECT

	public:
		DetectPlatformEditDialog(QWidget *parent);
		~DetectPlatformEditDialog();

		
	private slots:
		void acceptOk();
		void processDetectPlatformCreateResponse(const Datastruct::DetectPlatformCreateResponse & response);
	private:
		void init();
		void initConnect();
		void createNewDetectPlatform(QString name);
	private:
		QLineEdit * m_nameLineEdit;
	};

}//namespace Related 