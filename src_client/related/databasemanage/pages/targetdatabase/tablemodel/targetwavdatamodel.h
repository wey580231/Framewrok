/*! 
 * @brief     单个目标音频数据模型
 * @details 
 * @author    yzg
 * @version   1.0
 * @date      2021.02.03 15:05:42
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

	class TargetWavDataModel : public Base::RTableModel
	{
		Q_OBJECT

	public:
		TargetWavDataModel(QObject *parent = nullptr);
		~TargetWavDataModel();

		void prepareData(QList<Datastruct::TargetWavEntityData> listData);
		int datasSize() const;
	protected:
		QVariant displayData(int rowIndex, int dataIndex, int columnId) const;

	private:
		QList<Datastruct::TargetWavEntityData> m_dataList;

	};

}//namespace Related 