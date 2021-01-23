#include "pluginloader.h"

#include <QFileInfo>
#include <QDir>
#include <QPluginLoader>
#include <QDebug>

#include <base/util/rsingleton.h>
#include <base/util/rlog.h>
#include <base/selfwidget/rmessagebox.h>
#include <core/pluginmanager/plugin/pluginmanager.h>
#include <core/pluginmanager/interface/rcomponent.h>
#include <core/pluginmanager/interface/rtoolcomponent.h>
#include "../../file/programfilepath.h"

namespace Core{

PluginLoader::PluginLoader():parsemethod(NULL)
{
    m_pluginRespParseMethod = new PluginRespXmlParse();
    parsemethod = new PluginXmlParse;
}

PluginLoader * PluginLoader::m_instance = nullptr;

PluginLoader *PluginLoader::instance()
{
    if(m_instance == nullptr)
        m_instance = new PluginLoader();
    return m_instance;
}

/*!
 * @brief 从lib/plugins/目录下加载可用的dll文件
 */
void PluginLoader::loadPlugin()
{
    ProgramFilePath filePath;
    QDir dir(filePath.pluginPath);
    QStringList filters;
    filters<<"*.dll";
    QFileInfoList fileList = dir.entryInfoList(filters);

    QString errInfo;
    std::for_each(fileList.begin(),fileList.end(),[&](const QFileInfo & info){
        QPluginLoader loader(info.absoluteFilePath());
        RComponent * component = qobject_cast<RComponent *>(loader.instance());
        PluginDesc pdesc;
        pdesc.location = info.filePath();
        if(component){
            pdesc.loaded = true;
            pdesc.pluginName = component->pluginName();
            pdesc.pluginId = component->getPluginId();
            pdesc.groupId = "wait";
            pdesc.version = "1.0.0";
            PluginManager::instance()->addAvailblePlugin(component);
        }else{
            if(errInfo.isEmpty())
                errInfo = QStringLiteral("下列路径插件加载失败：\n");

            errInfo += info.absoluteFilePath();
            errInfo += "\n";
        }
        m_pluginSpecList.push_back(pdesc);
    });
#ifndef JZ_PROJ
    if(!errInfo.isEmpty())
        Base::RMessageBox::warning(nullptr,QStringLiteral("插件加载..."),errInfo, Base::RMessageBox::Ok);
#endif
}

/*!
 * @brief 从lib/tools/目录下加载可用的工具dll
 */
void PluginLoader::loadToolPlugin()
{
    ProgramFilePath filePath;
    QDir dir(filePath.toolPluginPath);
    QStringList filters;
    filters<<"*.dll";
    QFileInfoList fileList = dir.entryInfoList(filters);

    std::for_each(fileList.begin(),fileList.end(),[&](const QFileInfo & info){
        QPluginLoader loader(info.absoluteFilePath());
        RToolComponent * component = qobject_cast<RToolComponent *>(loader.instance());
        PluginDesc pdesc;
        pdesc.location = info.filePath();
        if(component){
            pdesc.loaded = true;
            pdesc.pluginName = component->pluginName();
            pdesc.version = "1.0.0";
            PluginManager::instance()->addAvailbleToolPlugin(component);
        }
    });
}

/*!
* @brief 加载pluginresp.xml获取所有插件集合
*/
bool PluginLoader::loadPluginXML()
{
    ProgramFilePath filePath;
    Base::RXmlFile file(filePath.pluginRespFile);
    file.setParseMethod(m_pluginRespParseMethod,false);
    return file.startParse();
}

/*!
* @brief 加载模块plugins.xml文件
*/
bool PluginLoader::loadModuleFile()
{
    ProgramFilePath filePath;
    Base::RXmlFile file(filePath.pluginFile);
    file.setParseMethod(parsemethod,false);
    return file.startParse();
}

/*!
* @brief 保存模块plugins.xml文件
*/
bool PluginLoader::saveModuleFile()
{
    ProgramFilePath filePath;
    Base::RXmlFile file(filePath.pluginFile);
    file.setParseMethod(parsemethod,false);
    return file.startSave();
}

Datastruct::NetworkInfo PluginLoader::getNetwork(QString networkId, bool &existed)
{
    existed = false;
    Datastruct::NetworkInfo info;

    NetworkMap * networkMap = parsemethod->getNetworks();
    if(networkMap && networkMap->contains(networkId)){
        info = networkMap->value(networkId);
        existed = true;
    }
    return info;
}

}
