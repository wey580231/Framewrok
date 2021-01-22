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

#include <base/util/rblockingqueue.h>
#include "datastruct.h"

namespace Related {

	extern Base::RBlockingQueque<RequestUnit *> G_RequestQuque;		/*!< 客户端请求队列 */


} //namespace Related 