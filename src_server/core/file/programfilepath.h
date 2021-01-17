/*!
 *  @brief     系统文件配置信息
 *  @details   定义了系统中各个文件夹路径、文件名全路径等信息
 *  @author    wey
 *  @version   1.0
 *  @date      2018.09.28
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note 20190328:wey:增加布局路径信息,调整变量为const类型
 */
#ifndef PROGRAMFILEPATH_H
#define PROGRAMFILEPATH_H

#include <QObject>
#include <core/core_global.h>

namespace Core{

class CORESHARED_EXPORT ProgramFilePath
{
public:
    ProgramFilePath();

    //目录信息
    const QString programPath;				  /*!< 可执行程序目录 */
    const QString configPath;                 /*!< 配置文件 */
    const QString stylePath;                  /*!< 样式表路径 */
    const QString translationPath;            /*!< 翻译文件 */
    const QString shortcutPath;               /*!< 快捷键路径 */
    const QString pluginPath;                 /*!< 插件路径 */
    const QString toolPluginPath;             /*!< 工具插件路径 */
    const QString recordPath;                 /*!< 运行中产生的输出文件集合,记录文件需按照项目及类型分类存储 */
    const QString layoutPath;                 /*!< 布局文件 */
    const QString pluginConfigPath;           /*!< plugin.xml文件存放路径 */
    const QString statusbarConfigPath;        /*!< statusbarconfig.xml文件存放路径 */

    //配置文件信息
    const QString configFile;                 /*!< 全局配置文件(插件也可用) */
    const QString pluginFile;                 /*!< 框架配置文件 */
    const QString layoutFile;                 /*!< 默认布局文件 */
    const QString pluginRespFile;             /*!< 所有可用插件文件 20190716 */
};

} //namespace Core

#endif // PROGRAMFILEPATH_H
