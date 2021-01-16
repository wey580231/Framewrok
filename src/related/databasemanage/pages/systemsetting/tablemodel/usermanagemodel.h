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

#include <base\selfwidget\tablemode\rtablemodel.h>
#include "../../../datastruct.h"

namespace Related {

	class UserManageModel : public Base::RTableModel
	{
		Q_OBJECT

	public:
		UserManageModel(QObject *parent = nullptr);
		~UserManageModel();

		void prepareData();
		int datasSize() const;

	protected:
		QVariant displayData(int rowIndex, int dataIndex, int columnId) const;

	private:
		QList<UserManageData> m_dataList;

	};

}//namespace Related 
