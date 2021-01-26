/*!
 * @brief	 值班日志表格数据模型  
 * @details
 * @author    yzg
 * @version   1.0
 * @date      2021.01.23 09:45:32
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

	class LogbookModel : public Base::RTableModel
	{
		Q_OBJECT

	public:
		LogbookModel(QObject *parent = nullptr);
		~LogbookModel();

		void prepareData(QList<Datastruct::DutyRecordEntityData> dutyRecordInfos);

		int datasSize() const;

	protected:
		QVariant displayData(int rowIndex, int dataIndex, int columnId) const;

	private:
		QList<Datastruct::DutyRecordEntityData> m_dataList;
	};

}//namespace Related 
