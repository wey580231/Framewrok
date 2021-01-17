/*!
 *  @brief     软件全局配置信息文件
 *  @details   1.定义软件中各个通用常量信息(尺寸、颜色、位置等)
 *  @author    wey
 *  @version   1.0
 *  @date      2019.08.14
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef APPCONFIG_H
#define APPCONFIG_H

#include <QColor>

#include "../core_global.h"

namespace Core{

namespace ProjectDesc {

/*!
 * @brief 获取项目描述信息
 * @return 项目信息
 */
extern CORESHARED_EXPORT QString GetProjectDesc();

/*!
* @brief 是否注册程序随系统启动而运行
* @param[in] autoStart true:随系统启动自动运行
*/
extern void RegistAppToSystemStartUp(bool autoStart = true);

}

/*!
 *  @brief 固定尺寸相关信息
 */
namespace RSize{

#define TITLEBAR_MIN_VIEW_HEIGHT    25          /*!< 标题栏折叠后最小显示高度(px) */
#define STATUSBAR_FIX_HEIGHT        30          /*!< 状态栏固定高度(px) */

} //namespace RSize

/*!
 *  @brief 固定颜色配置信息
 */
namespace Color{

} //namespace Color


} //namespace Core{


#endif // APPCONFIG_H
