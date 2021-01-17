/*!
 *  @brief     模块关系维护
 *  @details   临时拥有PluginManager的职责，待两者合并:20181108;
 *  @author    wey
 *  @version   1.0
 *  @date      2018.11.08
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note 20190328:wey:增加加载工具dll
 */
#ifndef PLUGINLOADER_H
#define PLUGINLOADER_H

#include <QList>

#include "../../core_global.h"
#include "pluginxmlparse.h"

namespace Core{

/*!
 * @brief 插件基本描述
 */
struct PluginDesc
{
    PluginDesc(){
        QString defaultValue = "-";
        loaded = false;
        pluginName = defaultValue;
        pluginId = defaultValue;
        version = defaultValue;
        location = defaultValue;
        groupId = defaultValue;
    }
    QString pluginName;     /*!< 插件名称 */
    QString pluginId;       /*!< 插件Id */
    QString version;        /*!< 版本号 */
    QString location;       /*!< dll对应的路径信息 */
    QString groupId;        /*!< 插件所属组id */
    bool loaded;            /*!< 插件是否可以被加载 */
};

class CORESHARED_EXPORT PluginLoader
{
public:
    static PluginLoader * instance();
    void loadPlugin();
    void loadToolPlugin();
    QList<PluginDesc> getPluginDesc(){return this->m_pluginSpecList;}

    bool loadPluginXML();

    bool loadModuleFile();
    bool saveModuleFile();

    ModuleList * getModules(){return parsemethod->getModules();}
    NetworkMap * getNetworks(){return parsemethod->getNetworks();}
    PluginMap * getPlugins(){return m_pluginRespParseMethod->getPlugins();}

    Datastruct::NetworkInfo getNetwork(QString networkId,bool & existed);

private:
    PluginLoader();
    static PluginLoader * m_instance;

    PluginXmlParse * parsemethod;                       /*!< 插件配置文件解析方式 */
    PluginRespXmlParse * m_pluginRespParseMethod;       /*!< 可用插件解析方式 */
    QList<PluginDesc> m_pluginSpecList;
};

}

#endif // PLUGINLOADER_H
