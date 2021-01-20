/*!
 * @brief       
 * @author    yzg
 * @version   1.0
 * @date      2021.01.20 13:35:51
 * @warning
 * @copyright NanJing RenGu.
 * @note
 */
 
#pragma once

#include <QObject>

#include <base\selfwidget\tablemode\rtablemodel.h>
#include "../../../datastruct.h"

namespace Related {

	class TargetDatabaseManageModel : public Base::RTableModel
	{
		Q_OBJECT

	public:
		TargetDatabaseManageModel(QObject *parent = nullptr);
		~TargetDatabaseManageModel();

		void prepareData();
		int datasSize() const;

	protected:
		QVariant displayData(int rowIndex, int dataIndex, int columnId) const;

	private:
		QList<TargetDatabaseData> m_dataList;
	};

}// namespace Related 