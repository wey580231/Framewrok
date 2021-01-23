/*!
 *  @brief     状态栏配置列表
 *  @details   配置状态栏各个容器、item可见状态信息;文件保存在config/statusbarconfig.xml中
 *  @author    wey
 *  @version   1.0
 *  @date      2019.08.16
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef STATUSBARITEMPARSEMETHOD_H
#define STATUSBARITEMPARSEMETHOD_H

#include <base/util/fileutils.h>
#include <base/actionmanager/statusbarcontainer.h>

namespace Core{

class StatusBarItemParseMethod : public Base::RXmlParseMethod
{
public:
    explicit StatusBarItemParseMethod();
    ~StatusBarItemParseMethod();

    bool  startParse(QDomNode & rootNode);
    bool  startSave(QDomDocument & doc);

    QList<Base::StatusBarContainerItem> getParseResult();
    void setSaveContainers(QList<Base::StatusBarContainer *> containers);

private:
    QList<Base::StatusBarContainer *> m_containers;
    QList<Base::StatusBarContainerItem> m_simpleContainersDesc;

};

} //namespace Core

#endif // STATUSBARITEMPARSEMETHOD_H
