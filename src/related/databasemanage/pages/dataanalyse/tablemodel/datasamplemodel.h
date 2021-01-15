/*!
 *  @brief    数据样本model
 *  @details   
 *  @author    yzg
 *  @version   1.0
 *  @date      2021.01.13 13:07
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#pragma once

#include <QObject>

#include <base\selfwidget\tablemode\rtablemodel.h>
#include "../../../datastruct.h"

namespace Related {

	class datasamplemodel : public Base::RTableModel
	{
		Q_OBJECT

	public:
		datasamplemodel(QObject *parent = nullptr);
		~datasamplemodel();

		int rowCount(const QModelIndex &parent /* = QModelIndex() */) const;

		void prepareData();

	protected:
		QVariant displayData(int rowIndex, int dataIndex, int columnId) const;
		QVariant customProcessRole(int role, int rowIndex, int dataIndex, int columnId) const;

	private:
		QList<Level0Data> m_dataList;

	};

}//namespace Related 