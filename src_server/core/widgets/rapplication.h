/*!
 *  @brief     接管Qt事件循环处理
 *  @details
 *  @author    wey
 *  @version   1.0
 *  @date      2020.08.15
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef RAPPLICATION_H
#define RAPPLICATION_H

#include <QApplication>

#include <core/core_global.h>

namespace Core {

class CORESHARED_EXPORT RApplication : public QApplication
{
    Q_OBJECT
public:
    RApplication(int &argc, char **argv);
    ~RApplication();

    bool notify(QObject * obj, QEvent * event) override;

};

} //namespace Core

#endif // RAPPLICATION_H
