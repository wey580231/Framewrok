#include "programfilepath.h"

#include <QDir>
#include <QApplication>

namespace Core{
ProgramFilePath::ProgramFilePath():
    programPath(qApp->applicationDirPath()),
    configPath(programPath + QDir::separator() + QString("/rconfig")),
    stylePath(configPath + QDir::separator() + QString("styles")),
    translationPath(configPath + QDir::separator() + QString("translations")),
    shortcutPath(configPath + QDir::separator() + QString("schemes")),
    layoutPath(configPath + QDir::separator() + QString("layouts")),
    pluginPath(programPath + QDir::separator() + QString("/rplugins")),
    toolPluginPath(programPath + QDir::separator() + QString("/rtools")),
    recordPath(programPath + QDir::separator() + QString("/record")),

    pluginConfigPath(configPath + QDir::separator() + QString("pluginxml/common")),

    configFile(configPath + QDir::separator() + QString("config.ini")),
    pluginFile(pluginConfigPath + QDir::separator() + QString("plugins.xml")),
    pluginRespFile(configPath + QDir::separator() + QString("pluginxml/pluginresp.xml")),
    layoutFile(layoutPath + QDir::separator() + QString("defaultlayout")),
    statusbarConfigPath(configPath + QDir::separator() + QString("statusbarconfig.xml"))
{

}

} //namespace Core
