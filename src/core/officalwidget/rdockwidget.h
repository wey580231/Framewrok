/*!
 *  @brief     扩展Qt官方Dock
 *  @details   在MyDockWidget基础上，增设QDockWidget子类。两种模式不可共存，可通过宏来区分
 *  @author    wey
 *  @version   1.0
 *  @date      2019.11.02
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef RDOCKWIDGET_H_2019_11_06
#define RDOCKWIDGET_H_2019_11_06

#include <QDockWidget>
#include "../core_global.h"

namespace Core{

class CORESHARED_EXPORT RDockWidget : public QDockWidget
{
    Q_OBJECT
public:
    explicit RDockWidget(QWidget * parent = nullptr);
    ~RDockWidget();

    /*!< 是否眼睛可见 */
    bool isEyeVisible(){return m_visibility;}

private slots:
    void updateVisibility(bool visible);

private:
    bool m_visibility;      /*!< 窗体是否可见 */

};

} //namespace Core{

#endif // RDOCKWIDGET_H
