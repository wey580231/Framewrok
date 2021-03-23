#include "global.h"

namespace Related {
	QString G_FileSaveRootPath;											/*!< 文件保存根路径 */

	Base::RBlockingQueque<RequestUnit *> G_RequestQuque;				/*!< 客户端请求队列 */
	Base::RBlockingQueque<FileRequestUnit *> G_FileRequestQuque;		/*!< 文件客户端请求队列 */

	QMap<QString, ReceiveDataFile * > G_MapWriteRawDataFile;

	 std::mutex g_mutex_FileRequestQueueMutex;
	 std::condition_variable g_cv_FileRequestQuqueCondition;
	 QQueue<FileRequestUnit *>  g_FileRequestQueue;

} //namespace Related 