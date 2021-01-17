/*!
 *  @brief     Office2013风格窗口
 *  @details   由Office风格菜单栏和可嵌入的QMainWindow组成
 *  @author    wey
 *  @version   1.0
 *  @date      2020.07.30
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef OFFICESTYLEWINDOW_H
#define OFFICESTYLEWINDOW_H

#include <QWidget>

#include <core/core_global.h>

namespace Core{

class OfficeMenuBar;
class MainWindow;
class AbstractOfficeMenuItem;

class CORESHARED_EXPORT OfficeStyleWindow : public QWidget
{
    Q_OBJECT
public:
    explicit OfficeStyleWindow(QWidget * parent = nullptr);
    static OfficeStyleWindow * instance();

    void initAll();
    void displayResize();

    void addMenu(AbstractOfficeMenuItem * item);

protected:
    void closeEvent(QCloseEvent * event);

private:
    void init();

private:
    static OfficeStyleWindow * m_instance;

    OfficeMenuBar * m_menuBar;
    MainWindow * m_mainWindow;

};

} //namespace Core

#endif // OFFICESTYLEWINDOW_H
