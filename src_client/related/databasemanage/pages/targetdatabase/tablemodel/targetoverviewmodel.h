/*!
 * @brief     目标预览表格数据模型   
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
#include <commondefines\protocol.h>

#include "../../../datastruct.h"

namespace Related {

	class TargetOverViewModel : public Base::RTableModel
	{
		Q_OBJECT

	public:
		TargetOverViewModel(QObject *parent = nullptr);
		~TargetOverViewModel();

		void prepareData(QList<Datastruct::TargetEntityData> listData);
		int datasSize() const;

	protected:
		QVariant displayData(int rowIndex, int dataIndex, int columnId) const;

	private:
		QList<Datastruct::TargetEntityData> m_dataList;
	};

}// namespace Related 