#include "taskrawdatafiledealthread.h"

#include <QDebug>

namespace Related {

	TaskRawDataFileDealThread::TaskRawDataFileDealThread(QObject *parent)
		: Core::RTask(parent)
	{
	}

	TaskRawDataFileDealThread::~TaskRawDataFileDealThread()
	{
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

		}
	}

	/*!
	 * @brief 
	 * @details 
	 */
	void TaskRawDataFileDealThread::dealFile()
	{
	}

}//namespace Related 