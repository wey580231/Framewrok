/*!
 *  @brief     布局解析与保存
 *  @details   布局中包含普通Widget、MyDockWidget、MyTabWidget三种控件，并按照这样顺序依次保存。
 *             其中Widget与MyDockWidget作为dockItems容器一并处理； @n
 *             MyTabWidget保存至tabItems容器 @n
 *  @author    wey
 *  @version   1.0
 *  @date      2018.11.16
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 *      20181203:wey:增加窗口可嵌入属性
 */
#ifndef LAYOUTPARSEMETHOD_H
#define LAYOUTPARSEMETHOD_H

#include <base/util/fileutils.h>

namespace Base {
class RTextFile;
}

namespace Core{

namespace Datastruct{
class DockItem;
class TabItem;
}

class LayoutParseMethod : public Base::RTextParseMethod
{
public:
    explicit LayoutParseMethod();
    ~LayoutParseMethod();

    void addDockItem(Datastruct::DockItem * item);
    void addTabItem(Datastruct::TabItem * item);
    QList<Datastruct::DockItem *> getDockItems(){return this->dockItems;}
    QList<Datastruct::TabItem *> getTabItems(){return this->tabItems;}

    bool  startParse(Base::RTextFile * file);
    bool  startSave(Base::RTextFile * file);

private:
    int magicNum;
    QList<Datastruct::DockItem *> dockItems;      /*!< 存放普通Widget与MyDockWidget */
    QList<Datastruct::TabItem *> tabItems;       /*!< 存放MyTabWidget */
};

}

#endif // LAYOUTPARSEMETHOD_H
