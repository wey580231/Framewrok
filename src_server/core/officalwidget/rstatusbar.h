/*!
 *  @brief     官方版StatusBar
 *  @details
 *  @author
 *  @version   1.0
 *  @date      2019.11.02
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef RSTATUSBAR_H
#define RSTATUSBAR_H

#include <QStatusBar>

namespace Core{

class RStatusBar : public QStatusBar
{
    Q_OBJECT
public:
    explicit RStatusBar(QWidget * parent = nullptr);
};

} //namespace Core

#endif // RSTATUSBAR_H
