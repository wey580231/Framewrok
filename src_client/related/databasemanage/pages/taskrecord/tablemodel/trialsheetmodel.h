/*!
 * @brief	  试验记录数据表格模型     
 * @author    yzg
 * @version   1.0
 * @date      2021.01.23 10:33:46
 * @warning
 * @copyright NanJing RenGu.
 * @note
 */
 
#pragma once

#include <QObject>

#include <base\selfwidget\tablemode\rtablemodel.h>
#include "../../../datastruct.h"

namespace Related {

	class TrialSheetModel : public  Base::RTableModel
	{
		Q_OBJECT

	public:
		TrialSheetModel(QObject *parent = nullptr);
		~TrialSheetModel();

		void prepareData();
		int datasSize() const;

	protected:
		QVariant displayData(int rowIndex, int dataIndex, int columnId) const;


	private:
		QList<TrialSheetData> m_dataList;

	};

}//namespace Related 