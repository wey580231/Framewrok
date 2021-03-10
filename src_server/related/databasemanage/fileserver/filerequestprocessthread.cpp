#include "filerequestprocessthread.h"

#include <QDebug>

#include <commondefines/protocol.h>
#include <base\util\rlog.h>

#include "../global.h"
#include "../datastruct.h"

namespace Related {

	FileRequestProcessThread::FileRequestProcessThread(QObject *parent)
		: Core::RTask(parent)
	{
	}

	FileRequestProcessThread::~FileRequestProcessThread()
	{
	}

	void FileRequestProcessThread::startMe()
	{
		RTask::startMe();
		runningFlag = true;
		start();
	}

	void FileRequestProcessThread::stopMe()
	{
		RTask::stopMe();
		runningFlag = false;
		G_FileRequestQuque.wakeUpConsumer(true);
	}

	void FileRequestProcessThread::run()
	{
		while (runningFlag) {

			while (runningFlag && G_FileRequestQuque.size() == 0) {
				G_FileRequestQuque.wait();
			}

			if (runningFlag) {
				//[1]
				std::list<FileRequestUnit *> dataList = G_FileRequestQuque.takeAll();

				std::for_each(dataList.begin(), dataList.end(), [&](FileRequestUnit * unit) {
					parseFileRequest(unit);
				});
			}
		}
	}

	void FileRequestProcessThread::parseFileRequest(FileRequestUnit * unit)
	{
	}

}////namespace Related 