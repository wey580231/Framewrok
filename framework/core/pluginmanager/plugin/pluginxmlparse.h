/*!
 *  @brief     插件xml文件解析
 *  @details   1.解析plugins.xml文件
 *             2.保存修改后的plugin.xml文件
 *  @author    wey
 *  @version   1.0
 *  @date      2018.11.08
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 *      20190111:wey:增加TCP网络记录节点
 */
#ifndef PLUGINXMLPARSE_H
#define PLUGINXMLPARSE_H

#include <core/protocol/datastruct.h>
#include <base/util/fileutils.h>

#include <QMap>

namespace Core{

using namespace Datastruct;

class PluginXmlParse : public Base::RXmlParseMethod
{
public:
    PluginXmlParse();

    bool  startParse(const QDomNode & rootNode);
    bool  startSave(QDomDocument & doc);

    enum RectPos{
        R_X,
        R_Y,
        R_W,
        R_H
    };

    ModuleList * getModules(){return this->modules;}
    NetworkMap * getNetworks(){return this->networks;}

private:
    void parseNetwork(const QDomNode &networkNode);
    void parseModule(const QDomNode &moduleNode);

    void saveNetwork(QDomDocument & doc,QDomElement & network);
    void saveModule(QDomDocument & doc, QDomElement & moduleNodess);

    int getRectSize(RectPos, QString content);

private:
    ModuleList * modules;
    NetworkMap * networks;

    QString splitter;
};

/*!
 *  @brief 解析pluginresp.xml，获取可用的插件集合
 */
class PluginRespXmlParse : public Base::RXmlParseMethod
{
public:
    PluginRespXmlParse();

    bool  startParse(QDomNode & rootNode);

    PluginMap * getPlugins(){return this->plugins;}

private:
    void parsePlugin(const QDomNode &pluginsNode);

private:
    PluginMap * plugins;
};

}

#endif // PLUGINXMLPARSE_H
