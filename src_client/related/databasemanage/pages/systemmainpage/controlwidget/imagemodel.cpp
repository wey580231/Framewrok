#include "imagemodel.h"

#include <QDateTime>
#include <QDebug>

#include <base/util/rutil.h>

#include "../../../datastruct.h"

namespace Related {

	ImageModel::ImageModel(QObject *parent)
		: Base::RTableModel(parent)
	{
		m_dataList.clear();
	}

	ImageModel::~ImageModel()
	{
	}

	int ImageModel::rowCount(const QModelIndex &parent /*= QModelIndex()*/) const
	{
		if (m_dataList.size() < Base::RTableModel::rowCount(parent)) {
			return Base::RTableModel::rowCount(parent);
		}
		else {
			return m_dataList.size();
		}
	}

	void ImageModel::updateData(QList<Datastruct::TaskImageEntityData> & fileInfos)
	{
		m_dataList.clear();
		m_dataList.append(fileInfos);
		refresh();
	}

	QList<Datastruct::TaskImageEntityData> ImageModel::getdatas()
	{
		return m_dataList;
	}

	void ImageModel::clearData()
	{
		m_dataList.clear();
		refresh();
	}

	QVariant ImageModel::displayData(int rowIndex, int dataIndex, int columnId) const
	{
		ImageTableColumn cindex = static_cast<ImageTableColumn>(columnId);
		if (dataIndex >= 0 && dataIndex < m_dataList.size()) {
			Datastruct::TaskImageEntityData fileInfo = m_dataList.at(dataIndex);

			switch (cindex)
			{
			case Img_Id:
				return dataIndex + 1;
			case Img_Name:
				return fileInfo.realName;
			case Img_FileTimeStamp:
				return fileInfo.uploadTime;
			case Img_FileType:
				return fileInfo.suffix;
			case Img_FileSize:
				return fileInfo.imageSize;
			default:
				break;
			}
		}

		return QVariant();
	}

	int ImageModel::datasSize() const
	{
		return m_dataList.size();
	}

} //namespace Related 
