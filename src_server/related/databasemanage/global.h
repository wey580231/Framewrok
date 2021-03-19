/*!
 * @brief     全局变量   
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

	extern QString G_FileSaveRootPath;										/*!< 文件保存根路径 */

	extern Base::RBlockingQueque<RequestUnit *> G_RequestQuque;				/*!< 客户端请求队列 */
	extern Base::RBlockingQueque<FileRequestUnit *> G_FileRequestQuque;		/*!< 文件客户端请求队列 */


	extern QMap<QString, ReceiveDataFile * > G_MapWriteRawDataFile;

	extern std::mutex g_mutex_FileRequestQueueMutex;
	extern std::condition_variable g_cv_FileRequestQuqueCondition;
	extern QQueue<FileRequestUnit *>  g_FileRequestQueue;

} //namespace Related 