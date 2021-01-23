/*!
 *  @brief     官方版工具栏
 *  @details   支持用户自定义工具栏，添加自定义的工具窗口
 *  @author    wey
 *  @version   1.0
 *  @date      2019.11.05
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef RTOOLBAR_H
#define RTOOLBAR_H

#include <QToolBar>
#include "../core_global.h"

namespace Core{

class CORESHARED_EXPORT RToolBar : public QToolBar
{
    Q_OBJECT
public:
    static RToolBar * instance();
    ~RToolBar();

private:
    explicit RToolBar(QWidget * parent = nullptr);

    static RToolBar * m_instance;

};

} //namespace Core

#endif // RTOOLBAR_H
