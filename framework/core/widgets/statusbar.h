/*!
 *  @brief     底部工具及状态栏
 *  @details   提供额外的操作按钮及状态显示，位置区可由插件扩展，主要参考qtccreator、notepad、word等通用软件
 *             用户可指定添加多个功能区，每个功能区包含唯一标识。
 *  @author    wey
 *  @version   1.0
 *  @date      2019.08.14
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef STATUSBAR_H
#define STATUSBAR_H

#include <base/actionmanager/statusbarcontainer.h>
#include "../selfwidget/widget.h"
#include "../core_global.h"

namespace Core{

class StatusBarPrivate;

class CORESHARED_EXPORT StatusBar : public Widget
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(StatusBar)
public:
    explicit StatusBar(QWidget * parent = 0);
    ~StatusBar();

    static StatusBar *instance();

    void fixedPosBottom();

    void loadLocalSetting();

    //容器操作
	Base::StatusBarContainer * container(Base::Id id);
    void addContainer(Base::Id id);
    void insertContainer(Base::Id beforeId, Base::Id id);

    void appendItem(Base::Id containerId, Base::StatusBarItem item);

protected:
    void mousePressEvent(QMouseEvent * event);
    void contextMenuEvent(QContextMenuEvent * event);

private slots:
    void containerItemVisibleChanged(Base::StatusBarContainer * container);
    void saveConfigXMLFile();

private:
    bool existedItem(Base::Id id);

private:
    StatusBarPrivate * d_ptr;
    static StatusBar * m_statusBar;
};

} //namespace Core

#endif // STATUSBAR_H
