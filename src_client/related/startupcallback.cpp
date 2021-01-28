#include "startupcallback.h"

#include <QDebug>

#include <core/widgets/mainwindow.h>

#include "databasemanage\frame\mainwindow.h"

namespace Related {

/*!
* @brief 主窗体初始化前调用
*/
void BeforeStartUpCallback()
{

}

/*!
* @brief 主窗体初始化后调用
*/
void AfterStartUpCallback()
{
	MainWindow  * window = new MainWindow();
	Core::MainWindow::instance()->setCentralWidget(window);
	Core::MainWindow::instance()->setMinimumSize(1600,900);
}

} //namespace Related
