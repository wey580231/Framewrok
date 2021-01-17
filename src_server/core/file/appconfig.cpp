#include "appconfig.h"

#include <QApplication>
#include <QDir>
#include <QSettings>

#include <core/constants.h>

namespace Core{

namespace ProjectDesc {

CORESHARED_EXPORT QString GetProjectDesc()
{
#ifdef INFRARED_PROJ
    return QStringLiteral("红外隐身系统");
#endif
	return QString();
}

void RegistAppToSystemStartUp(bool autoStart)
{
#ifdef Q_OS_WIN
    QString t_registKey = "HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run";
    QSettings sett(t_registKey,QSettings::NativeFormat);
    if(autoStart){
        sett.setValue(Constant::ApplicationName,QDir::toNativeSeparators(QCoreApplication::applicationFilePath()));
    }else{
        sett.remove(Constant::ApplicationName);
    }
    sett.sync();
#endif
}

} //namespace ProjectDesc

} //namespace Core
