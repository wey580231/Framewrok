#include <QTextCodec>
#include <QDebug>
#include <QSplashScreen>
#include <QSettings>
#include <QDir>

#include <base/util/rutil.h>
#include <core/constants.h>
#include <core/file/appconfig.h>
#include <core/widgets/rapplication.h>
#include <related/startupcallback.h>

#ifdef ENABLE_OFFICE_MENU
#include <core/widgets/officestylewindow.h>
#else
#include <core/widgets/mainwindow.h>
#endif

#include "coredump.h"

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

int main(int argc, char *argv[])
{
    App::EnableRecordDump();

    Core::RApplication a(argc, argv);

    QApplication::setApplicationName(Constant::ApplicationName);
    QApplication::setOrganizationDomain(QObject::tr("rengu.com"));
    QApplication::setOrganizationName(QObject::tr("NanJing RenGu"));
    QApplication::setApplicationVersion(Constant::Version);

#if QT_VERSION > 0x050000
    QTextCodec * codec = QTextCodec::codecForName("utf-8");
    QTextCodec::setCodecForLocale(codec);
#endif

    //加载并显示启动画面
    QSplashScreen splash(QPixmap(":/welcome/resource/welcome/welcome.jpg"));
    splash.setDisabled(true);
    splash.show();
    QFont font;
    font.setFamily("微软雅黑");
    font.setPixelSize(50);
    font.setBold(true);
    font.setPointSize(20);
    font.setWeight(QFont::Light);
    splash.setFont(QFont(font));
    splash.showMessage(Core::ProjectDesc::GetProjectDesc(),Qt::AlignHCenter|Qt::AlignBottom,Qt::black);

    Related::BeforeStartUpCallback();

#ifdef ENABLE_OFFICE_MENU
    Core::OfficeStyleWindow w;
    w.initAll();
#else
    Core::MainWindow w;
    w.initAll();
#endif

    //若直接show(),则会出现窗口初始为非全屏,需要双击后才能切换至全屏
    w.showMinimized();
    Related::AfterStartUpCallback();

    w.displayResize();

    splash.finish(&w);

    return a.exec();
}
