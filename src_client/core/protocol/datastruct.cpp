#include "datastruct.h"
#include <QDate>
#include <QDebug>
namespace Core{

namespace Datastruct{

/********************布局信息*************************/
//BaseLayoutItem
QDataStream & operator << (QDataStream & stream,BaseLayoutItem & item)
{
    stream<<item.objName<<item.geometry<<item.feature;
    return stream;
}

QDataStream & operator >> (QDataStream & stream,BaseLayoutItem & item)
{
    stream>>item.objName>>item.geometry>>item.feature;
    return stream;
}

//TabInfo
QDataStream & operator << (QDataStream &stream, TabInfo &item)
{
    stream<<item.tabbed<<item.tabObjName;
    return stream;
}

QDataStream & operator >> (QDataStream &stream, TabInfo &item)
{
    stream>>item.tabbed>>item.tabObjName;
    return stream;
}

//DockItem
QDataStream & operator << (QDataStream &stream, DockItem &item)
{
    BaseLayoutItem * p = dynamic_cast<DockItem *>(&item);
    stream<<*p;

    stream<<item.objName<<item.geometry<<item.feature<<item.expanded<<item.tabInfo;
    return stream;
}

QDataStream & operator >> (QDataStream &stream, DockItem &item)
{
    BaseLayoutItem * p = dynamic_cast<DockItem *>(&item);
    stream>>*p;

    stream>>item.objName>>item.geometry>>item.feature>>item.expanded>>item.tabInfo;
    return stream;
}

//TabItem
QDataStream & operator << (QDataStream &stream, TabItem &item)
{
    BaseLayoutItem * p = dynamic_cast<BaseLayoutItem *>(&item);
    stream<<*p;

    stream<<item.tabPosition<<item.currentIndex<<item.tabPageObjNames;
    return stream;
}

QDataStream & operator >> (QDataStream &stream, TabItem &item)
{
    BaseLayoutItem * p = dynamic_cast<BaseLayoutItem *>(&item);
    stream>>*p;

    stream>>item.tabPosition>>item.currentIndex>>item.tabPageObjNames;
    return stream;
}

} //namespace Datastruct

} //namespace Core
