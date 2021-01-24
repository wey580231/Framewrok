/*!
 * @brief     数据预览表格数据模型
 * @details 
 * @author    yzg
 * @version   1.0
 * @date      2021.01.23 15:03:49
 * @warning
 * @copyright NanJing RenGu.
 * @note
 */
 
#pragma once

#include <QObject>

#include <base\selfwidget\tablemode\rtablemodel.h>
#include "../../../datastruct.h"

namespace Related {

	class DataOverviewMolel : public Base::RTableModel
	{
		Q_OBJECT

	public:
		DataOverviewMolel(QObject *parent = nullptr);
		~DataOverviewMolel();

		void prepareData();
		int datasSize() const;
	protected:
		QVariant displayData(int rowIndex, int dataIndex, int columnId) const;

	private:
		QList<DataOverviewData> m_dataList;
	};

}//namespace Related 