#include "filedescriptionmodel.h"

#include <QDebug>

namespace Related {

	FileDescriptionModel::FileDescriptionModel(QObject *parent)
		: Base::RTableModel(parent)
	{
		m_dataList.clear();
	}

	FileDescriptionModel::~FileDescriptionModel()
	{
	}

	int FileDescriptionModel::rowCount(const QModelIndex &parent /*= QModelIndex()*/) const
	{
		if (m_dataList.size() < Base::RTableModel::rowCount(parent)) {
			return Base::RTableModel::rowCount(parent);
		}
		else {
			return m_dataList.size();
		}
	}

	void FileDescriptionModel::updateData(QList<FileDescriptionData> & fileInfos)
	{
		m_dataList.clear();
		m_dataList.append(fileInfos);
		refresh();
	}

	QList<FileDescriptionData> FileDescriptionModel::getdatas()
	{
		return m_dataList;
	}

	void FileDescriptionModel::clearData()
	{
		m_dataList.clear();
		refresh();
	}

	int FileDescriptionModel::datasSize() const
	{
		return m_dataList.size();
	}

	QVariant FileDescriptionModel::displayData(int rowIndex, int dataIndex, int columnId) const
	{
		FileDescriptionTableColumn cindex = static_cast<FileDescriptionTableColumn>(columnId);
		if (dataIndex >= 0 && dataIndex < m_dataList.size()) {
			FileDescriptionData fileDescription = m_dataList.at(dataIndex);

			switch (cindex)
			{
			case FileDescription_Index:
				return dataIndex + 1;
			case FileDescription_Name:
				return fileDescription.m_name;
			case FileDescription_FileTimeStamp:
				return fileDescription.m_fileTimeStamp;
			case FileDescription_FileType:
				return fileDescription.m_suffix;
			case FileDescription_FileSize:
				return Base::RUtil::switchBytesUnit(fileDescription.m_size);  
			default:
				break;
			}
		}
		return QVariant();
	}

} //namespace Related 
