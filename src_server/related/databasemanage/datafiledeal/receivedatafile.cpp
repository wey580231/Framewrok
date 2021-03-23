#include "receivedatafile.h"

#include <QDebug>

namespace Related {

	ReceiveDataFile::ReceiveDataFile(QObject *parent)
		: QObject(parent),
		m_dataProcess(nullptr)
	{

	}

	ReceiveDataFile::~ReceiveDataFile()
	{
	}

	void ReceiveDataFile::bindDataProcess(DataProcessCenter * dataProcess)
	{
		m_dataProcess = dataProcess;
	}

	void ReceiveDataFile::setTaskId(QString taskId)
	{
		m_taskId = taskId;
	}

	void ReceiveDataFile::setFileType(Datastruct::FileType type)
	{
		m_fileType = type;
	}

	void ReceiveDataFile::setFileName(QString rootPath ,QString fileName, QString suffix)
	{
		QString filePath;
		if (m_fileType == Datastruct::File_Image) {
			filePath = QString("%1/%2/%3").arg(rootPath).arg(m_taskId).arg(QStringLiteral("image"));
		}
		else if(m_fileType == Datastruct::P_FileData || m_fileType == Datastruct::File_XML)
		{
			filePath = QString("%1/%2/%3").arg(rootPath).arg(m_taskId).arg(QStringLiteral("data"));

			m_resolveFilePath = QString("%1/%2/%3").arg(rootPath).arg(m_taskId).arg(QStringLiteral("resolvedata"));
		}
		//
		Base::RUtil::createDir(filePath);
		Base::RUtil::createDir(m_resolveFilePath);

		m_filePathName = QString("%1/%2.%3").arg(filePath).arg(fileName).arg(suffix);

		openFile(m_filePathName);
	}

	void ReceiveDataFile::setFileData(Datastruct::FileInfoParameter parameter, QByteArray data)
	{	
		if (m_file != nullptr) {
			m_file->write(data);
			m_file->flush();
			qDebug() << "___________setFileData____________________" << parameter.m_totalLength << parameter.m_currentLength << data.size();

			if (parameter.m_currentLength >= parameter.m_totalLength  ) {
				//[1] 关闭文件
				closeFile();
				//[2] 对数据文件进行处理
				if (m_fileType == Datastruct::P_FileData || m_fileType == Datastruct::File_XML) {
					m_originalDataDeal = new OriginalDataDealRunnable();
					m_originalDataDeal->setDataFilePath(m_filePathName);
					m_originalDataDeal->setResolveDataFilePath(m_resolveFilePath);
					QThreadPool::globalInstance()->start(m_originalDataDeal);
				}	
			}
		}
	}

	bool ReceiveDataFile::openFile(QString fileName)
	{
		m_file = new QFile(fileName);
		bool t_open = m_file->open(QIODevice::WriteOnly);
		if (!t_open) {
			return false;
		}
		return true;
	}

	void ReceiveDataFile::closeFile()
	{
		m_file->close();
	}
}//namespace Related
