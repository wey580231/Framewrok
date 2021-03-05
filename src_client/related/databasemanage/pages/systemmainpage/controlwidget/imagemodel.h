/*!
 * @brief     任务图片表格数据模型   
 * @author    wey
 * @version   1.0
 * @date      2021.01.26 16:02:39
 * @warning
 * @copyright NanJing RenGu.
 * @note
 */
#pragma once

#include <QObject>
#include <QFileInfo>

#include <base\selfwidget\tablemode\rtablemodel.h>
#include <commondefines\protocol.h>

#include "../../../datastruct.h"

namespace Related {

	class ImageModel : public Base::RTableModel
	{
		Q_OBJECT

	public:
		ImageModel(QObject *parent = nullptr);
		~ImageModel();

		int rowCount(const QModelIndex &parent = QModelIndex()) const;

		void updateData(QList<Datastruct::TaskImageEntityData> & fileInfos);
		QList<Datastruct::TaskImageEntityData> getdatas();

		void clearData();
		int datasSize() const;

	protected:
		QVariant displayData(int rowIndex, int dataIndex, int columnId) const;

	private:
		QList<Datastruct::TaskImageEntityData> m_dataList;
	};

} //namespace Related 
