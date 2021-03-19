/*!
 * @brief    写原始文件
 * @details 
 * @author    yzg
 * @version   1.0
 * @date      2021.03.10 19:12:39
 * @warning
 * @copyright NanJing RenGu.
 * @note
 */
 #pragma once

#include <QObject>
#include <QFile>
#include <QByteArray>

#include <base/util/rutil.h>
#include <commondefines/protocol.h>

#include "../business/dataprocesscenter.h"

namespace Related {

class ReceiveDataFile : public QObject
{
	Q_OBJECT

public:
	ReceiveDataFile(QObject *parent = nullptr);
	~ReceiveDataFile();

	enum FileType{
		File_Image,
		File_Data,
	};

	void bindDataProcess(DataProcessCenter * dataProcess);

	void setTaskId(QString taskId);
	void setFileType(FileType type);
	void setFileName(QString rootPath, QString fileName, QString suffix);


	void seFileData(Datastruct::FileInfoParameter parameter, QByteArray data);

private:
	bool openFile(QString fileName);
	void closeFile();

private:
	DataProcessCenter * m_dataProcess;	
	QString m_rootPath;

	QString  m_taskId;					/*!< 任务Id */
	FileType m_fileType;				/*!< 文件类型 */
	QString m_fileId;
	QFile * m_file;

	int m_saveFileIndex;
};

}//namespace Related