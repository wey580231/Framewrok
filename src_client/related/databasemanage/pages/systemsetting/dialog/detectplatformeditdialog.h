/*!
 * @brief     侦查平台/侦查平台亚型编辑子界面
 * @details   该界面用于新增侦查平台或平台亚型信息
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

		enum EditType {
			E_DetectPlatform,							/*!< 平台 */
			E_DetectPlatformSubtype,					/*!< 平台亚型 */
		};

		void setEditType(EditType type);
		void setPlatformId(int platformId);
		
	private slots:
		void acceptOk();
		void processDetectPlatformCreateResponse(const Datastruct::DetectPlatformCreateResponse & response);
		void processDetectPlatformSubtypeCreateResponse(const Datastruct::DetectPlatformSubtypeCreateResponse & response);

	private:
		void init();
		void initConnect();
		void createNewDetectPlatform(QString name);
		void createNewDetectPlatformSubtype(QString name, int platformId);

	private:
		QLineEdit * m_nameLineEdit;

		EditType m_editType;							/*!< 编辑类型 */
		int m_platformId;								/*!< 平台Id */
	};

}//namespace Related 