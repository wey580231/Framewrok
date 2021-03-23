/*!
 * @brief    дԭʼ�ļ�
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
	QString  m_taskId;					/*!< ����Id */
	Datastruct::FileType m_fileType;	/*!< �ļ����� */
	QString m_fileId;					/*!< �ļ�Id */
	QString m_filePathName;				/*!< �ļ�·�� */
	QString m_resolveFilePath;			/*!< ������ɵ��ļ�·�� */

	QFile * m_file;						/*!< �ļ���� */

	OriginalDataDealRunnable * m_originalDataDeal;	
};

}//namespace Related