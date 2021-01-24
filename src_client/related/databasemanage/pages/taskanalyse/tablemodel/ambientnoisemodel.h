/*!
 * @brief	  环境噪声数据表格模型  
 * @details 
 * @author    yzg
 * @version   1.0
 * @date      2021.01.23 16:06:28
 * @warning
 * @copyright NanJing RenGu.
 * @note
 */
 
#pragma once
#include <QObject>

#include <base\selfwidget\tablemode\rtablemodel.h>

#include "../../../datastruct.h"

namespace Related {

	class AmbientNoiseModel : public Base::RTableModel
	{
		Q_OBJECT

	public:
		AmbientNoiseModel(QObject *parent = nullptr);
		~AmbientNoiseModel();

		void prepareData();

		int datasSize() const;

	protected:
		QVariant displayData(int rowIndex, int dataIndex, int columnId) const;
	
	private:
		QList<AmbientNoiseData> m_dataList;

	};

}//namespace Related
