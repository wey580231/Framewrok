/*!
 *  @brief     web版地图的操作窗口
 *  @details   负责加载地图
 *  @author
 *  @version   1.0
 *  @date      2020.07.31
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef MAPVIEW_H_2020_08_04
#define MAPVIEW_H_2020_08_04

#include <QWebEngineView>
#include <QProcess>

#include <core/core_global.h>

class QProcess;

namespace Core{

namespace MapWeb2D{

class CORESHARED_EXPORT MapView : public QWebEngineView
{
    Q_OBJECT
public:
    explicit MapView(QWidget *parent = nullptr);
    ~MapView();

    void initWebPage();
    void startWebService();

private slots:
    void respProcessState(QProcess::ProcessState state);
    void sreadProcessOutput();
    void respProcessError(QProcess::ProcessError error);

private:
    QProcess * m_process;

    QString configPath;
};

} //namespace MapWeb2D

} //namespace Core

#endif // MAPVIEW_H
