/*!
 * @brief     侦测平台亚型数据模型
 * @details 
 * @author    yzg
 * @version   1.0
 * @date      2021.01.28 19:37:35
 * @warning
 * @copyright NanJing RenGu.
 * @note
 */
#pragma once

#include <QObject>

#include <base\selfwidget\tablemode\rtablemodel.h>
#include <commondefines\protocol.h>

#include "../../../datastruct.h"

namespace Related {

	class DetectPlatformSubtypeModel : public Base::RTableModel
	{
		Q_OBJECT

	public:
		DetectPlatformSubtypeModel(QObject *parent = nullptr);
		~DetectPlatformSubtypeModel();

		int datasSize() const;

		void updateData(const QList<Datastruct::DetectPlatformSubtypeEntityData> & dlist);

		int getRecordDatabaseIndex(int row);

	protected:
		QVariant displayData(int rowIndex, int dataIndex, int columnId) const;

	private:
		QList<Datastruct::DetectPlatformSubtypeEntityData>  m_dataList;

	};

}//namespace Related 