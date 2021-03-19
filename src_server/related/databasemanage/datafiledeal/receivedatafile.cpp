#include "receivedatafile.h"

#include <QDebug>

namespace Related {

	ReceiveDataFile::ReceiveDataFile(QObject *parent)
		: QObject(parent),
		m_dataProcess(nullptr),
		m_saveFileIndex(0)
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

	void ReceiveDataFile::setFileType(FileType type)
	{
		m_fileType = type;
	}

	void ReceiveDataFile::setFileName(QString rootPath ,QString fileName, QString suffix)
	{
		QString filePath;
		if (m_fileType == File_Image) {
			filePath = QString("%1/%2/%3").arg(rootPath).arg(m_taskId).arg(QStringLiteral("image"));
		}
		else if(m_fileType == File_Data)
		{
			filePath = QString("%1/%2/%3").arg(rootPath).arg(m_taskId).arg(QStringLiteral("data"));
		}
		//
		Base::RUtil::createDir(filePath);

		QString t_allFileName = QString("%1/%2.%3").arg(filePath).arg(fileName).arg(suffix);

		openFile(t_allFileName);
	}

	void ReceiveDataFile::seFileData(Datastruct::FileInfoParameter parameter, QByteArray data)
	{	
		if (m_file != nullptr) {
			m_file->write(data);
			m_file->flush();
			m_saveFileIndex += parameter.m_currentLength;
			if (m_saveFileIndex == parameter.m_totalLength) {
				closeFile();
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
