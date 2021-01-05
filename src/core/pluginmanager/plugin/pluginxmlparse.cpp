#include "pluginxmlparse.h"

#include <QDebug>

#include "Base/util/rutil.h"

namespace Core{

struct PluginNode{
    QString node = "plugin";
    QString id = "id";
    QString name = "name";
};

struct NetworkNode{
    QString node = "network";
    QString id = "id";
    QString type = "type";
    QString ip = "ip";
    QString port = "port";
    QString protocol = "protocol";
    QString multicast = "multicast";
    QString multiIp = "multiIp";
    QString recordFile = "recordfile";
    QString recordPath = "recordPath";
    QString fileSize = "filesize";
};

struct MoudleNode{
    QString node = "moudle";
    QString id = "id";
    QString closeable = "closeable";
    QString visible = "visible";
    QString floatable = "floatable";
    QString name = "name";
    QString protocols = "protocols";
    QString pluginId = "plugin";
    QString networkId = "network";
    QString recv = "recv";
    QString send = "send";
    QString layout = "layout";
    QString geometry = "geometry";
    QString x = "x";
    QString y = "y";
    QString w = "w";
    QString h = "h";
};

PluginXmlParse::PluginXmlParse():splitter(";")
{

}

bool PluginXmlParse::startParse(const QDomNode &rootNode)
{
    //network
    QDomNodeList nets = rootNode.toElement().elementsByTagName("net");
    if(nets.size() != 1)
        return false;
    parseNetwork(nets.at(0));

    //module
    QDomNodeList moudles = rootNode.toElement().elementsByTagName("moudles");
    if(moudles.size() != 1)
        return false;
    parseModule(moudles.at(0));

    return true;
}

bool PluginXmlParse::startSave(QDomDocument &doc)
{
    QDomElement rootElement = doc.createElement("root");

    //network
    QDomElement nets = doc.createElement("net");
    saveNetwork(doc,nets);
    rootElement.appendChild(nets);

    //modules
    QDomElement modules = doc.createElement("moudles");
    saveModule(doc,modules);
    rootElement.appendChild(modules);

    doc.appendChild(rootElement);

    return true;
}

void PluginXmlParse::parseNetwork(const QDomNode &networkNode)
{
    QDomNodeList nodes = networkNode.toElement().childNodes();
    NetworkNode pnode;
    networks = new NetworkMap;
    for(int i = 0; i < nodes.size();i++){
        QDomNode child = nodes.at(i).firstChild();
        Datastruct::NetworkInfo netinfo;
        while(!child.isNull()){
            QDomElement childElement = child.toElement();
            QString tagname = childElement.tagName();
            if(tagname == pnode.id){
                netinfo.id = childElement.text();
            }else if(tagname == pnode.ip){
                netinfo.baseInfo.ip = childElement.text();
            }else if(tagname == pnode.port){
                netinfo.baseInfo.port = childElement.text().toUShort();
            }else if(tagname == pnode.protocol){
                if(childElement.text().toLower() == "udp"){
                    netinfo.protocol = Datastruct::N_UDP;
                }else if(childElement.text().toLower() == "tcp"){
                    netinfo.protocol = Datastruct::N_TCP;
                }
            }else if(tagname == pnode.type){
                if(childElement.text().toLower() == "client"){
                    netinfo.baseInfo.connectionType = Datastruct::N_Client;
                }else if(childElement.text().toLower() == "server"){
                    netinfo.baseInfo.connectionType = Datastruct::N_Server;
                }
            }else if(tagname == pnode.multicast){
                netinfo.baseInfo.multicast = childElement.text().toInt();
            }else if(tagname == pnode.multiIp){
                netinfo.baseInfo.multicastIp = childElement.text();
            }else if(tagname == pnode.recordFile){
                netinfo.baseInfo.recordNetFile = childElement.text().toInt();
            }else if(tagname == pnode.recordPath){
                netinfo.baseInfo.recordNetPath = childElement.text();
            }else if(tagname == pnode.fileSize){
                netinfo.baseInfo.singleFileSize = childElement.text().toInt();
            }
            child = child.nextSibling();
        }
        networks->insert(netinfo.id,netinfo);
    }
}

void PluginXmlParse::parseModule(const QDomNode &moduleNode)
{
    QDomNodeList nodes = moduleNode.toElement().childNodes();
    MoudleNode pnode;
    modules = new ModuleList;
    for(int i = 0; i < nodes.size();i++){
        QDomNode child = nodes.at(i).firstChild();
        Datastruct::ModuleInfo minfo;
        while(!child.isNull()){
            QDomElement childElement = child.toElement();
            QString tagname = childElement.tagName();
            if(tagname == pnode.closeable)
            {
                minfo.closeable = childElement.text().toInt();
            }
            else if(tagname == pnode.visible)
            {
                minfo.visible = childElement.text().toInt();
            }
            else if(tagname == pnode.floatable)
            {
                minfo.floatable = childElement.text().toInt();
            }
            else if(tagname == pnode.name)
            {
                minfo.name = childElement.text();
            }
            else if(tagname == pnode.pluginId)
            {
                minfo.pluginId = childElement.text();
            }
            else if(tagname == pnode.networkId)
            {
                QDomNode netChild = child.firstChild();
                NetworkConfig t_netConfig;
                while(!netChild.isNull()){
                    QDomElement netElement = netChild.toElement();
                    QString gname = netElement.tagName();
                    if(gname == pnode.protocols){
                        t_netConfig.protocols = netElement.text().split(splitter);
                    }else if(gname == pnode.recv)
                        t_netConfig.recvNetworkId = netElement.text();
                    else if(gname == pnode.send)
                        t_netConfig.sendsNetworkId = netElement.text();
                    netChild = netChild.nextSibling();
                }
                minfo.netconfigs.append(t_netConfig);
            }
            else if(tagname == pnode.layout)
            {
                QString layout = childElement.text().toLower();
                if(layout == "left"){
                    minfo.layout = Datastruct::LEFT;
                }else if(layout == "top"){
                    minfo.layout = Datastruct::TOP;
                }else if(layout == "right"){
                    minfo.layout = Datastruct::RIGHT;
                }else if(layout == "bottom"){
                    minfo.layout = Datastruct::BOTTOM;
                }
            }
            else if(tagname == pnode.geometry)
            {
                QDomNode geometryChild = child.firstChild();
                while(!geometryChild.isNull()){
                    QDomElement geometryElement = geometryChild.toElement();
                    QString gname = geometryElement.tagName();
                    if(gname == pnode.x)
                    {
                        minfo.geometry.setX(getRectSize(R_X,geometryElement.text()));
                    }
                    else if(gname == pnode.y)
                    {
                        minfo.geometry.setY(getRectSize(R_Y,geometryElement.text()));
                    }
                    else if(gname == pnode.w)
                    {
                        minfo.geometry.setWidth(getRectSize(R_W,geometryElement.text()));
                    }
                    else if(gname == pnode.h)
                    {
                        minfo.geometry.setHeight(getRectSize(R_H,geometryElement.text()));
                    }
                    geometryChild = geometryChild.nextSibling();
                }
            }
            child = child.nextSibling();
        }
        modules->push_back(minfo);
    }
}

void PluginXmlParse::saveNetwork(QDomDocument &doc, QDomElement &network)
{
    NetworkMap::iterator iter = networks->begin();
    NetworkNode pnode;
    while(iter != networks->end()){
        QDomElement netNode = doc.createElement(pnode.node);

        QDomElement id = doc.createElement(pnode.id);
        id.appendChild(doc.createTextNode(iter.value().id));

        QDomElement ip = doc.createElement(pnode.ip);
        ip.appendChild(doc.createTextNode(iter.value().baseInfo.ip));

        QDomElement port = doc.createElement(pnode.port);
        port.appendChild(doc.createTextNode(QString::number(iter.value().baseInfo.port)));

        QString protocolText;
        switch(iter.value().protocol){
            case Datastruct::N_TCP:
                protocolText = "tcp";break;
            case Datastruct::N_UDP:
                protocolText = "udp";break;
            default:break;
        }
        QDomElement protocol = doc.createElement(pnode.protocol);
        protocol.appendChild(doc.createTextNode(protocolText));

        QString typeText;
        switch(iter.value().baseInfo.connectionType){
            case Datastruct::N_Client:
                typeText = "client";break;
            case Datastruct::N_Server:
                typeText = "server";break;
            default:break;
        }

        QDomElement type = doc.createElement(pnode.type);
        type.appendChild(doc.createTextNode(typeText));

        netNode.appendChild(id);
        netNode.appendChild(ip);
        netNode.appendChild(port);
        netNode.appendChild(protocol);
        netNode.appendChild(type);

        if(iter.value().protocol == Datastruct::N_UDP){
            QDomElement isMulti = doc.createElement(pnode.multicast);
            isMulti.appendChild(doc.createTextNode(QString::number((int)iter.value().baseInfo.multicast)));

            QDomElement multiIp = doc.createElement(pnode.multiIp);
            multiIp.appendChild(doc.createTextNode(iter.value().baseInfo.multicastIp));

            netNode.appendChild(isMulti);
            netNode.appendChild(multiIp);
        }else if(iter.value().protocol == Datastruct::N_TCP){
            QDomElement recordFile = doc.createElement(pnode.recordFile);
            recordFile.appendChild(doc.createTextNode(QString::number((int)iter.value().baseInfo.recordNetFile)));

            QDomElement recordPath = doc.createElement(pnode.recordPath);
            recordPath.appendChild(doc.createTextNode(iter.value().baseInfo.recordNetPath));

            QDomElement fileSize = doc.createElement(pnode.fileSize);
            fileSize.appendChild(doc.createTextNode(QString::number(iter.value().baseInfo.singleFileSize)));

            netNode.appendChild(recordFile);
            netNode.appendChild(recordPath);
            netNode.appendChild(fileSize);
        }
        network.appendChild(netNode);
        iter++;
    }
}

void PluginXmlParse::saveModule(QDomDocument &doc, QDomElement &moduleNodess)
{
    ModuleList::iterator iter = modules->begin();
    MoudleNode pnode;
    while(iter != modules->end()){
        QDomElement moduleNode = doc.createElement(pnode.node);
        Datastruct::ModuleInfo info = *iter;

        //layout
        QDomElement layout = doc.createElement(pnode.layout);
        QString layoutText;

        switch(info.layout){
            case Datastruct::TOP:layoutText = "top"; break;
            case Datastruct::BOTTOM:layoutText = "bottom"; break;
            case Datastruct::LEFT:layoutText = "left"; break;
            case Datastruct::RIGHT:layoutText = "right"; break;
            default:break;
        }
        layout.appendChild(doc.createTextNode(layoutText));

        //geometry
        QDomElement geometry = doc.createElement(pnode.geometry);
        QDomElement x = doc.createElement(pnode.x);
        x.appendChild(doc.createTextNode(QString::number(info.geometry.x())));
        QDomElement y = doc.createElement(pnode.y);
        y.appendChild(doc.createTextNode(QString::number(info.geometry.y())));
        QDomElement w = doc.createElement(pnode.w);
        w.appendChild(doc.createTextNode(QString::number(info.geometry.width())));
        QDomElement h = doc.createElement(pnode.h);
        h.appendChild(doc.createTextNode(QString::number(info.geometry.height())));

        geometry.appendChild(x);
        geometry.appendChild(y);
        geometry.appendChild(w);
        geometry.appendChild(h);

        //close
        QDomElement closeable = doc.createElement(pnode.closeable);
        closeable.appendChild(doc.createTextNode(QString::number((int)info.closeable)));

        //visible
        QDomElement visible = doc.createElement(pnode.visible);
        visible.appendChild(doc.createTextNode(QString::number((int)info.visible)));

        //floatable
        QDomElement floatable = doc.createElement(pnode.floatable);
        floatable.appendChild(doc.createTextNode(QString::number((int)info.floatable)));

        //name
        QDomElement name = doc.createElement(pnode.name);
        name.appendChild(doc.createTextNode(info.name));

        //plugin
        QDomElement pluginId = doc.createElement(pnode.pluginId);
        pluginId.appendChild(doc.createTextNode(info.pluginId));

        //network
        std::for_each(info.netconfigs.begin(),info.netconfigs.end(),[&](NetworkConfig & config){

            QDomElement networkId = doc.createElement(pnode.networkId);

            QDomElement protocols = doc.createElement(pnode.protocols);
            protocols.appendChild(doc.createTextNode(config.protocols.join(splitter)));
            networkId.appendChild(protocols);

            QDomElement recv = doc.createElement(pnode.recv);
            recv.appendChild(doc.createTextNode(config.recvNetworkId));
            networkId.appendChild(recv);

            QDomElement send = doc.createElement(pnode.send);
            send.appendChild(doc.createTextNode(config.sendsNetworkId));

            networkId.appendChild(send);

            moduleNode.appendChild(networkId);
        });

        moduleNode.appendChild(layout);
        moduleNode.appendChild(geometry);
        moduleNode.appendChild(closeable);
        moduleNode.appendChild(visible);
        moduleNode.appendChild(floatable);
        moduleNode.appendChild(name);
        moduleNode.appendChild(pluginId);

        moduleNodess.appendChild(moduleNode);
        iter++;
    }
}

int PluginXmlParse::getRectSize(RectPos pos,QString content)
{
    if(content.indexOf("%") >= 0){
        int percentInt = content.left(content.indexOf("%")).toInt();
        if(percentInt < 0 || percentInt >100)
            percentInt = 0;

        double percent = (double)percentInt/100;
        switch(pos){
            case R_X:
            case R_W:
                return Base::RUtil::screenGeometry().width() * percent;
                break;

            case R_Y:
            case R_H:
                return Base::RUtil::screenGeometry().height() * percent;
                break;

            default:
                break;
        }
    }else{
        return content.toInt();
    }

    return 0;
}

PluginRespXmlParse::PluginRespXmlParse()
{

}

bool PluginRespXmlParse::startParse(QDomNode &rootNode)
{
    //plugin
    QDomNodeList plugins = rootNode.toElement().elementsByTagName("plugins");
    if(plugins.size() != 1)
        return false;
    parsePlugin(plugins.at(0));

    return true;
}

void PluginRespXmlParse::parsePlugin(const QDomNode &pluginsNode)
{
    QDomNodeList nodes = pluginsNode.toElement().childNodes();
    PluginNode pnode;
    plugins = new PluginMap;
    for(int i = 0; i < nodes.size();i++){
        QDomNode child = nodes.at(i).firstChild();
        Datastruct::PluginInfo pinfo;
        while(!child.isNull()){
            QDomElement childElement = child.toElement();
            QString tagname = childElement.tagName();
            if(tagname == pnode.id)
            {
                pinfo.id = childElement.text();
            }
            else if(tagname == pnode.name)
            {
                pinfo.name = childElement.text();
            }
            child = child.nextSibling();
        }
        plugins->insert(pinfo.id,pinfo);
    }
}

}
