#include "layoutparsemethod.h"

#include <QDataStream>
#include <QDebug>

#include <core/protocol/datastruct.h>

namespace Core{

LayoutParseMethod::LayoutParseMethod()
{
    magicNum = 0xABCDDCBA;
}

LayoutParseMethod::~LayoutParseMethod()
{
    std::for_each(dockItems.begin(),dockItems.end(),[&](Datastruct::DockItem * item){
        delete item;
    });

    std::for_each(tabItems.begin(),tabItems.end(),[&](Datastruct::TabItem * item){
        delete item;
    });
}

void LayoutParseMethod::addDockItem(Datastruct::DockItem *item)
{
    dockItems.push_back(item);
}

void LayoutParseMethod::addTabItem(Datastruct::TabItem *item)
{
    tabItems.push_back(item);
}

bool LayoutParseMethod::startParse(Base::RTextFile *file)
{
    QDataStream stream(file);
    int tmpMagic;
    stream>>tmpMagic;
    if(tmpMagic != magicNum)
        return false;

    int size = 0;
    stream>>size;

    for(int i = 0;i < size; i++){
        Datastruct::DockItem  * item = new Datastruct::DockItem;
        stream>>*item;
        dockItems.push_back(item);
    }

    size = 0;
    stream>>size;

    for(int i = 0;i < size; i++){
        Datastruct::TabItem  * item = new Datastruct::TabItem;
        stream>>*item;
        tabItems.push_back(item);
    }

    return true;
}

/*!
 * @brief 保存布局数据
 */
bool LayoutParseMethod::startSave(Base::RTextFile *file)
{
    QDataStream stream(file);
    stream<<magicNum;

    stream<<dockItems.size();

    std::for_each(dockItems.begin(),dockItems.end(),[&](Datastruct::DockItem * item){
        stream<<*item;
    });

    stream<<tabItems.size();

    std::for_each(tabItems.begin(),tabItems.end(),[&](Datastruct::TabItem * item){
        stream<<*item;
    });

    return true;
}

}
