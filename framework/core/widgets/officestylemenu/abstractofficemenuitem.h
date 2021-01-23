/*!
 *  @brief     Office菜单项
 *  @details   继承此类,并实现对应的方法,可通过统一的接口注册到菜单项上
 *  @author
 *  @version   1.0
 *  @date      2020.07.
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef OFFICERMENUITEM_H
#define OFFICERMENUITEM_H

#include <QObject>

#include <core/core_global.h>

namespace Core{

class CORESHARED_EXPORT AbstractOfficeMenuItem : public QObject
{
    Q_OBJECT
public:
    explicit AbstractOfficeMenuItem(QObject * parent = nullptr);
    virtual ~AbstractOfficeMenuItem();

    /*!
     * @brief 返回菜单项中的名称
     */
    virtual QString menuName() = 0;

    virtual QWidget * menuContainer(){return m_container;}

protected:
    QWidget * m_container;

};

} //namespace Core

#endif // OFFICERMENUITEM_H
