#include "taskrawdatafiledealthread.h"

#include <QDebug>
#include <QFileInfo>
#include <QFile>

#include "../../net/filenetconnector.h"

namespace Related {

	TaskRawDataFileDealThread::TaskRawDataFileDealThread(QObject *parent)
		: Core::RTask(parent), 
		m_totalFileSize(0)
	{
	}

	TaskRawDataFileDealThread::~TaskRawDataFileDealThread()
	{
	}

	void TaskRawDataFileDealThread::setTaskId(QString taskId)
	{
		m_taskId = taskId;
	}

	void TaskRawDataFileDealThread::setFilePats(QStringList pathList)
	{
		m_rawDataList = pathList;
	}

	void TaskRawDataFileDealThread::startMe()
	{
		RTask::startMe();
		runningFlag = true;
		start();
	}

	void TaskRawDataFileDealThread::stopMe()
	{
		RTask::stopMe();
		runningFlag = false;
	}

	void TaskRawDataFileDealThread::run()
	{
		while (runningFlag) {
			
			dealFile();

			stopMe();
		}
	}

	void TaskRawDataFileDealThread::dealFile()
	{
		//[] 总书文件大小
		foreach(QString filePath, m_rawDataList) {
			QFileInfo fileInfo(filePath);
			m_totalFileSize += fileInfo.size();
		}

		foreach(QString filePath, m_rawDataList) {
			dealSingleFile(filePath);
		}
	}

	void TaskRawDataFileDealThread::dealSingleFile(QString filePath)
	{
		QFile file(filePath);
		if (file.exists() && file.open(QIODevice::ReadOnly)){
			QByteArray t_array;
			while (!file.atEnd()) {
				t_array.clear();
				t_array = file.read(1024 * 1024 * 2);
				sendNewTaskRawDataFile(t_array);
			}
			file.close();
		}
	}

	void Related::TaskRawDataFileDealThread::sendNewTaskRawDataFile(QByteArray buff)
	{
		FileNetConnector::instance()->write(buff);

		qDebug() << "______________________________" << buff.size();
	}

}//namespace Related 