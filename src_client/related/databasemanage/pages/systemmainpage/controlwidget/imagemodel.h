/*!
 * @brief     ����ͼƬ��������ģ��   
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

namespace Related {

	class ImageModel : public Base::RTableModel
	{
		Q_OBJECT

	public:
		ImageModel(QObject *parent = nullptr);
		~ImageModel();

		int rowCount(const QModelIndex &parent = QModelIndex()) const;

		void updateData(QList<QFileInfo> & fileInfos);
		void clearData();
		int datasSize() const;

	protected:
		QVariant displayData(int rowIndex, int dataIndex, int columnId) const;

	private:
		QList<QFileInfo> m_dataList;
	};

} //namespace Related 