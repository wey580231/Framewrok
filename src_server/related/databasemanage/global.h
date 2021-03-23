/*!
 * @brief     ȫ�ֱ���   
 * @author    wey
 * @version   1.0
 * @date      2021.01.22 13:46:14
 * @warning
 * @copyright NanJing RenGu.
 * @note
 */
 #pragma once

#include <QObject>
#include <QQueue>
#include <QMap>

#include <base/util/rblockingqueue.h>
#include "datastruct.h"

#include "datafiledeal/receivedatafile.h"

namespace Related {

	extern QString G_FileSaveRootPath;										/*!< �ļ������·�� */

	extern Base::RBlockingQueque<RequestUnit *> G_RequestQuque;				/*!< �ͻ���������� */
	extern Base::RBlockingQueque<FileRequestUnit *> G_FileRequestQuque;		/*!< �ļ��ͻ���������� */


	extern QMap<QString, ReceiveDataFile * > G_MapWriteRawDataFile;

	extern std::mutex g_mutex_FileRequestQueueMutex;
	extern std::condition_variable g_cv_FileRequestQuqueCondition;
	extern QQueue<FileRequestUnit *>  g_FileRequestQueue;

} //namespace Related 