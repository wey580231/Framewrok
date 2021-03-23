#include "checktaskdatafile.h"

#include <QDebug>
#include <QDir>

namespace Related {

	CheckTaskDataFile::CheckTaskDataFile(QObject *parent)
		: QObject(parent)
	{
		m_listFileNames.clear();
	}

	CheckTaskDataFile::~CheckTaskDataFile()
	{
	}

	/*!
	 * @brief  ���������ļ���·��
	 * @details 
	 */
	void CheckTaskDataFile::setRawDataFileRootPath(QString path)
	{
		m_rootPath = path;
	}

	bool CheckTaskDataFile::checkRawDataDir()
	{
		// �ж��ļ�·���Ƿ���Ч
		QDir dir(m_rootPath);
		if (!dir.exists()){

			return false;
		}
		//
		dealRawDataDir(m_rootPath);

		return true;
	}

	QStringList CheckTaskDataFile::getFileLists()
	{
		return m_listFileNames;
	}


	bool CheckTaskDataFile::dealRawDataDir(QString path)
	{
		QDir dir(path);
		if (!dir.exists()) {
			return false;
		}

		QStringList filters;
		filters << QStringLiteral("*.xml") << QStringLiteral("*.xml") << QStringLiteral("*.dat");

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