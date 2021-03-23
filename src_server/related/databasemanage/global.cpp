#include "global.h"

namespace Related {
	QString G_FileSaveRootPath;											/*!< �ļ������·�� */

	Base::RBlockingQueque<RequestUnit *> G_RequestQuque;				/*!< �ͻ���������� */
	Base::RBlockingQueque<FileRequestUnit *> G_FileRequestQuque;		/*!< �ļ��ͻ���������� */

	QMap<QString, ReceiveDataFile * > G_MapWriteRawDataFile;

	 std::mutex g_mutex_FileRequestQueueMutex;
	 std::condition_variable g_cv_FileRequestQuqueCondition;
	 QQueue<FileRequestUnit *>  g_FileRequestQueue;

} //namespace Related 