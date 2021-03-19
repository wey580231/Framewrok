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
#include <QThreadPool>

#include <base/util/rutil.h>
#include <commondefines/protocol.h>

#include "../business/dataprocesscenter.h"
#include "originaldatadealrunnable.h"

namespace Related {

class ReceiveDataFile : public QObject
{
	Q_OBJECT

public:
	ReceiveDataFile(QObject *parent = nullptr);
	~ReceiveDataFile();

	void bindDataProcess(DataProcessCenter * dataProcess);

	void setTaskId(QString taskId);
	void setFileType(Datastruct::FileType type);
	void setFileName(QString rootPath, QString fileName, QString suffix);

	void setFileData(Datastruct::FileInfoParameter parameter, QByteArray data);

private:
	bool openFile(QString fileName);
	void closeFile();

private:
	DataProcessCenter * m_dataProcess;	
	QString  m_taskId;					/*!< 任务Id */
	Datastruct::FileType m_fileType;	/*!< 文件类型 */
	QString m_fileId;					/*!< 文件Id */
	QString m_filePathName;				/*!< 文件路径 */
	QString m_resolveFilePath;			/*!< 处理完成的文件路径 */

	QFile * m_file;						/*!< 文件句柄 */

	OriginalDataDealRunnable * m_originalDataDeal;	
};

}//namespace Related