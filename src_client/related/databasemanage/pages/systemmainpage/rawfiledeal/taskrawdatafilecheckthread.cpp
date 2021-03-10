#include "taskrawdatafilecheckthread.h"

#include <QDebug>
#include <QDir>

namespace Related {

	TaskRawDataFileCheckThread::TaskRawDataFileCheckThread(QObject *parent)
		: QObject(parent)
	{
		m_listFileNames.clear();
	}

	TaskRawDataFileCheckThread::~TaskRawDataFileCheckThread()
	{
	}

	/*!
	 * @brief  设置数据文件根路径
	 * @details 
	 */
	void TaskRawDataFileCheckThread::setRawDataFileRootPath(QString path)
	{
		m_rootPath = path;
	}

	bool TaskRawDataFileCheckThread::checkRawDataDir()
	{
		// 判断文件路径是否有效
		QDir dir(m_rootPath);
		if (!dir.exists()){

			return false;
		}
		//
		dealRawDataDir(m_rootPath);

		return true;
	}

	QStringList TaskRawDataFileCheckThread::getFileLists()
	{
		return m_listFileNames;
	}


	bool TaskRawDataFileCheckThread::dealRawDataDir(QString path)
	{
		QDir dir(path);
		if (!dir.exists()) {
			return false;
		}

		QStringList filters;
		filters << QStringLiteral("*.xlsx") << QStringLiteral("*.xls") << QStringLiteral("*.dat");

		QFileInfoList fileList = dir.entryInfoList(filters, QDir::AllDirs | QDir::Files);

		foreach(QFileInfo fileInfo, fileList)
		{
			if (fileInfo.fileName() == "." || fileInfo.fileName() == "..")
				continue;
			//
			if (fileInfo.isDir())
			{
				if (!dealRawDataDir(fileInfo.filePath()))
					return false;
			}
			else
			{
				
				m_listFileNames.append(fileInfo.filePath());
			}
		}
		return true;
	}

}//namespace Related 