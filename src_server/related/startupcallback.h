/*!
 *  @brief     程序启动时，在主窗体启动前、后添加启动步骤
 *  @details
 *  @author    wey
 *  @version   1.0
 *  @date      2019.12.13
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef STARTUPCALLBACK_H
#define STARTUPCALLBACK_H

#include "related_global.h"

namespace Related {

extern void RELATEDSHARED_EXPORT BeforeStartUpCallback();

extern void RELATEDSHARED_EXPORT AfterStartUpCallback();

} //namespace Related

#endif // STARTUPCALLBACK_H
