#include "filerequestprocessthread.h"

#include <QDebug>

#include <commondefines/protocol.h>
#include <commondefines/wrapper/jsonwrapper.h>
#include <base\util\rlog.h>

#include "../global.h"
#include "../datastruct.h"

using namespace CommonDefines;

namespace Related {

	FileRequestProcessThread::FileRequestProcessThread(QObject *parent)
		: Core::RTask(parent), m_dbConnect(nullptr)
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
		//G_FileRequestQuque.wakeUpConsumer(true);
		g_cv_FileRequestQuqueCondition.notify_all();
	}

	void FileRequestProcessThread::run()
	{

// 		while (runningFlag) {
// 
// 			while (runningFlag && G_FileRequestQuque.size() == 0) {
// 				G_FileRequestQuque.wait();
// 			}
// 
// 			if (runningFlag) {
// 				//[1]
// 				std::list<FileRequestUnit *> dataList = G_FileRequestQuque.takeAll();
// 
// 				std::for_each(dataList.begin(), dataList.end(), [&](FileRequestUnit * unit) {
// 					parseFileRequest(unit);
// 
// 					//delete unit;
// 				});
// 			}
// 		}

		while (runningFlag) {
			while (runningFlag && g_FileRequestQueue.size() == 0) {
				std::unique_lock<std::mutex> lg(g_mutex_FileRequestQueueMutex);
				g_cv_FileRequestQuqueCondition.wait(lg);
			}

			while (runningFlag && g_FileRequestQueue.size() > 0) {
				g_mutex_FileRequestQueueMutex.lock();
				FileRequestUnit * unit = g_FileRequestQueue.dequeue();
				g_mutex_FileRequestQueueMutex.unlock();
				//[]����������Ϣ
				parseFileRequest(unit);
				//[] �ͷ���Ϣ
				//delete unit;
			}
		}
	}

	/*!
	 * @brief �����ļ�����������
	 * @details unit �ļ���������Ԫ
	 */
	void FileRequestProcessThread::parseFileRequest(FileRequestUnit * unit)
	{
		Datastruct::PacketHead head;
		memcpy((char *)&head, unit->m_requestData.data(), sizeof(Datastruct::PacketHead));
		
		switch (head.m_packetType)
		{
		case Datastruct::P_FileData: {

			Datastruct::FileInfoParameter Parameter;
			memcpy((char *)&Parameter, unit->m_requestData.data() + sizeof(Datastruct::PacketHead), sizeof(Datastruct::FileInfoParameter));

			QByteArray data(unit->m_requestData.data() + sizeof(Datastruct::PacketHead) + sizeof(Datastruct::FileInfoParameter), 
				unit->m_requestData.size() - sizeof(Datastruct::PacketHead) - sizeof(Datastruct::FileInfoParameter) - sizeof(Datastruct::PacketTail));

			if (G_MapWriteRawDataFile.size() > 0) {
				if (G_MapWriteRawDataFile.contains(QString::fromLocal8Bit(Parameter.m_fileId))) {
					ReceiveDataFile * dataFile = G_MapWriteRawDataFile.find(QString::fromLocal8Bit(Parameter.m_fileId)).value();
					dataFile->setFileData(Parameter, data);
				}
			}
		}
		  break;

		default:
			break;
		}
	}

}////namespace Related 