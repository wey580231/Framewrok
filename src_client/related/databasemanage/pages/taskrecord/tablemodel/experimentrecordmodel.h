/*!
 * @brief	  试验记录数据表格模型     
 * @details
 * @author    yzg
 * @version   1.0
 * @date      2021.01.23 10:33:46
 * @warning
 * @copyright NanJing RenGu.
 * @note     20210128  完善表字段
 */
#pragma once

#include <QObject>

#include <base\selfwidget\tablemode\rtablemodel.h>
#include <commondefines\protocol.h>

#include "../../../datastruct.h"

namespace Related {

	class ExperimentRecordModel : public  Base::RTableModel
	{
		Q_OBJECT

	public:
		ExperimentRecordModel(QObject *parent = nullptr);
		~ExperimentRecordModel();

		void prepareData(QList<Datastruct::ExperimentRecordEntityData> listInfos);
		int datasSize() const;

	protected:
		QVariant displayData(int rowIndex, int dataIndex, int columnId) const;

	private:
		QList<Datastruct::ExperimentRecordEntityData> m_dataList;
	};

}//namespace Related 