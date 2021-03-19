/*!
 * @brief     任务描述表格数据模型   
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
#include <QDateTime>

#include <base\selfwidget\tablemode\rtablemodel.h>
#include <base/util/rutil.h>

#include "../../../datastruct.h"

namespace Related {

	class FileDescriptionModel : public Base::RTableModel
	{
		Q_OBJECT

	public:
		FileDescriptionModel(QObject *parent = nullptr);
		~FileDescriptionModel();

		int rowCount(const QModelIndex &parent = QModelIndex()) const;

		void updateData(QList<FileDescriptionData> & fileInfos);
		QList<FileDescriptionData> getdatas();

		void clearData();
		int datasSize() const;

	protected:
		QVariant displayData(int rowIndex, int dataIndex, int columnId) const;

	private:
		QList<FileDescriptionData> m_dataList;
	};

} //namespace Related 
