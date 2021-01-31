/*!
 * @brief     AIS表格数据模型
 * @details 
 * @author    yzg
 * @version   1.0
 * @date      2021.01.31 20:27:56
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

	class AisDataModel : public  Base::RTableModel
	{
		Q_OBJECT

	public:
		AisDataModel(QObject *parent = nullptr);
		~AisDataModel();

		void prepareData(QList<Datastruct::AisEntityData> listData);

		int datasSize() const;

	protected:
		QVariant displayData(int rowIndex, int dataIndex, int columnId) const;

	private:
		QList<Datastruct::AisEntityData> m_dataList;
	};

} //namespace Related 