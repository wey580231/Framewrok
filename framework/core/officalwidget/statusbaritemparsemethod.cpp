#include "statusbaritemparsemethod.h"

#include <QDebug>

namespace Core{

struct RootNode{
    QString rootNode = "Root";
    QString containerNode = "Containers";
};

struct ContainerNode{
    QString node = "Container";
    QString id = "id";
};

struct ItemNode{
    QString node = "Item";
    QString id = "Id";
    QString visible = "Visible";
    QString text = "Text";
};



StatusBarItemParseMethod::StatusBarItemParseMethod()
{

}

StatusBarItemParseMethod::~StatusBarItemParseMethod()
{

}

bool StatusBarItemParseMethod::startParse(QDomNode &rootNode)
{
    m_simpleContainersDesc.clear();

    RootNode rnode;
    QDomNodeList containerNodes = rootNode.toElement().elementsByTagName(rnode.containerNode);
    if(containerNodes.size() != 1)
        return false;

    ContainerNode cnode;
    ItemNode inode;

    QDomNodeList nodes = containerNodes.at(0).toElement().childNodes();
    for(int i = 0; i < nodes.size();i++){
		Base::StatusBarContainerItem titem;

        QDomElement tContainerEle = nodes.at(i).toElement();
        titem.m_id = Base::Id(tContainerEle.attribute(cnode.id).toLocal8Bit().data());

        QDomNodeList titems = tContainerEle.childNodes();
        for(int j = 0; j < titems.size(); j++){
			Base::StatusBarItem tbaritem;

            QDomElement tItemEle = titems.at(j).toElement();
            tbaritem.m_id = Base::Id(tItemEle.attribute(inode.id).toLocal8Bit().data());
            tbaritem.m_bVisible = tItemEle.attribute(inode.visible).toInt();
            tbaritem.m_text = tItemEle.attribute(inode.text);

            titem.m_items.append(tbaritem);
        }
        m_simpleContainersDesc.append(titem);
    }

    return true;
}

bool StatusBarItemParseMethod::startSave(QDomDocument &doc)
{
    RootNode rnode;
    QDomElement rootElement = doc.createElement(rnode.rootNode);

    QDomElement containers = doc.createElement(rnode.containerNode);

    ContainerNode cnode;
    ItemNode inode;

    for(int i = 0; i < m_containers.size(); i++){
		Base::StatusBarContainer * con = m_containers.at(i);

        QDomElement container = doc.createElement(cnode.node);
        container.setAttribute(cnode.id,con->id().toString());

        QList<Base::StatusBarItem> tItems = con->items();
        for(int j = 0; j < tItems.size(); j++){
            Base::StatusBarItem titem = tItems.at(j);

            QDomElement item = doc.createElement(inode.node);
            item.setAttribute(inode.id,titem.m_id.toString());
            item.setAttribute(inode.visible,titem.m_bVisible);
            item.setAttribute(inode.text,titem.m_text);

            container.appendChild(item);
        }

        containers.appendChild(container);
    }

    rootElement.appendChild(containers);
    doc.appendChild(rootElement);

    return true;
}

QList<Base::StatusBarContainerItem> StatusBarItemParseMethod::getParseResult()
{
    return m_simpleContainersDesc;
}

void StatusBarItemParseMethod::setSaveContainers(QList<Base::StatusBarContainer *> containers)
{
    m_containers = containers;
}

} //namespace Core
