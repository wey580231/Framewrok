/*!
 * @brief     用户管理表格数据模型
 * @author    yzg
 * @version   1.0
 * @date      2021.01.13 13:45:13
 * @warning
 * @copyright NanJing RenGu.
 * @note
 */
#pragma once

#include <QObject>

#include <commondefines\protocol.h>
#include <base\selfwidget\tablemode\rtablemodel.h>
#include "../../../datastruct.h"

namespace Related {

	class UserManageModel : public Base::RTableModel
	{
		Q_OBJECT

	public:
		UserManageModel(QObject *parent = nullptr);
		~UserManageModel();

		int datasSize() const;

		void updateData(const QList<Datastruct::UserEntityData> & dlist);

	protected:
		QVariant displayData(int rowIndex, int dataIndex, int columnId) const;

	private:
		QList<Datastruct::UserEntityData> m_dataList;

	};

}//namespace Related 
