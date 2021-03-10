#include "global.h"

namespace Related {

	Base::RBlockingQueque<RequestUnit *> G_RequestQuque;				/*!< 客户端请求队列 */
	Base::RBlockingQueque<FileRequestUnit *> G_FileRequestQuque;		/*!< 文件客户端请求队列 */
} //namespace Related 