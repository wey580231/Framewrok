#include "detectplatformsubtypemodel.h"

namespace Related {

	DetectPlatformSubtypeModel::DetectPlatformSubtypeModel(QObject *parent)
		: Base::RTableModel(parent)
	{

	}

	DetectPlatformSubtypeModel::~DetectPlatformSubtypeModel()
	{

	}

	int DetectPlatformSubtypeModel::datasSize() const
	{
		return 0;
	}

	void DetectPlatformSubtypeModel::updateData(const QList<Datastruct::DetectPlatformSubtypeEntityData>& dlist)
	{
	}

	int DetectPlatformSubtypeModel::getRecordDatabaseIndex(int row)
	{
		return 0;
	}

	QVariant DetectPlatformSubtypeModel::displayData(int rowIndex, int dataIndex, int columnId) const
	{
		return QVariant();
	}


}//namespace Related 
