#include "globalconfigfile.h"

#include <base/util/rutil.h>
#include <base/util/rlog.h>
#include <core/constants.h>

#include <QDebug>

namespace Core{

GlobalConfigFile::GlobalConfigFile():settings(nullptr)
{

}

bool GlobalConfigFile::parseFile()
{
    if(settings == nullptr)
        return false;

    //解析日志模块
    logConfig.isRecord2File = Base::RUtil::getGlobalValue(Constant::LOG_LOG,Constant::LOG_LOG_RECORDFILE,true).toBool();
    logConfig.level = Base::RUtil::getGlobalValue(Constant::LOG_LOG,Constant::LOG_LOG_LEVEL,RLog::RINFO).toInt();

    //数据库配置
    databaseConfig.hostName = Base::RUtil::getGlobalValue(Constant::DB_SETTING,Constant::DB_HOST,databaseConfig.hostName).toString();
    databaseConfig.dbName = Base::RUtil::getGlobalValue(Constant::DB_SETTING,Constant::DB_DATABASE_NAME,databaseConfig.dbName).toString();
    databaseConfig.dbUser = Base::RUtil::getGlobalValue(Constant::DB_SETTING,Constant::DB_USERNAME,databaseConfig.dbUser).toString();
    databaseConfig.dbPass = Base::RUtil::getGlobalValue(Constant::DB_SETTING,Constant::DB_PASSWORD,databaseConfig.dbPass).toString();
    databaseConfig.port = Base::RUtil::getGlobalValue(Constant::DB_SETTING,Constant::DB_PORT,databaseConfig.port).toInt();

    //系统配置
    systemConfigInfo.defaultKeySchemes = Base::RUtil::getGlobalValue(Constant::SYSTEM_SETTING,Constant::SYSTEM_DEFAULT_SCHEMES,systemConfigInfo.defaultKeySchemes).toBool();
    systemConfigInfo.userKeySchemesName = Base::RUtil::getGlobalValue(Constant::SYSTEM_SETTING,Constant::SYSTEM_USER_SCHEMES_NAME,systemConfigInfo.userKeySchemesName).toString();
    systemConfigInfo.locale = Base::RUtil::getGlobalValue(Constant::SYSTEM_SETTING,Constant::SYSTEM_LOCALE,systemConfigInfo.locale).toString();
    systemConfigInfo.style = Base::RUtil::getGlobalValue(Constant::SYSTEM_SETTING,Constant::SYSTEM_STYLE,systemConfigInfo.style).toString();
    systemConfigInfo.fullscreen = Base::RUtil::getGlobalValue(Constant::SYSTEM_SETTING,Constant::SYSTEM_FULLSCREEN,systemConfigInfo.fullscreen).toBool();
    systemConfigInfo.topHint = Base::RUtil::getGlobalValue(Constant::SYSTEM_SETTING,Constant::SYSTEM_TOPHINT,systemConfigInfo.topHint).toBool();
    systemConfigInfo.menubarVisible = Base::RUtil::getGlobalValue(Constant::SYSTEM_SETTING,Constant::SYSTEM_MENUBAR,systemConfigInfo.menubarVisible).toBool();
    systemConfigInfo.layout = Base::RUtil::getGlobalValue(Constant::SYSTEM_SETTING,Constant::SYSTEM_LAYOUT,systemConfigInfo.layout).toString();
    systemConfigInfo.m_bStatusBarVisible = Base::RUtil::getGlobalValue(Constant::SYSTEM_SETTING,Constant::SYSTEM_STATUSBAR,systemConfigInfo.m_bStatusBarVisible).toBool();
    systemConfigInfo.m_bNetworkAutoCreateInStartProcess = Base::RUtil::getGlobalValue(Constant::SYSTEM_SETTING,Constant::SYSTEM_NET_AUTOSTART,systemConfigInfo.m_bNetworkAutoCreateInStartProcess).toBool();
    systemConfigInfo.m_bDebugNetParsedInfo = Base::RUtil::getGlobalValue(Constant::SYSTEM_SETTING,Constant::SYSTEM_NET_DEBUG,systemConfigInfo.m_bDebugNetParsedInfo).toBool();
    systemConfigInfo.m_bAppAutoStart = Base::RUtil::getGlobalValue(Constant::SYSTEM_SETTING,Constant::SYSTEM_AUTO_START,true).toBool();

    //海图配置
    mapConfigInfo.useHtmlMap = Base::RUtil::getGlobalValue(Constant::MAP_SETTING,Constant::MAP_USEMAPHTML,mapConfigInfo.useHtmlMap).toString();
    mapConfigInfo.mapPath = Base::RUtil::getGlobalValue(Constant::MAP_SETTING,Constant::MAP_MAPPATH,mapConfigInfo.mapPath).toString();
    mapConfigInfo.refreshInterval = Base::RUtil::getGlobalValue(Constant::MAP_SETTING,Constant::MAP_REFRESHINTERVAL,mapConfigInfo.refreshInterval).toInt();

    return true;
}

void GlobalConfigFile::saveFile()
{
    if(settings == nullptr)
        return;

    settings->beginGroup(Constant::LOG_LOG);
    settings->setValue(Constant::LOG_LOG_RECORDFILE,logConfig.isRecord2File);
    settings->setValue(Constant::LOG_LOG_LEVEL,logConfig.level);
    settings->endGroup();

    settings->beginGroup(Constant::DB_SETTING);
    settings->setValue(Constant::DB_HOST,databaseConfig.hostName);
    settings->setValue(Constant::DB_DATABASE_NAME,databaseConfig.dbName);
    settings->setValue(Constant::DB_USERNAME,databaseConfig.dbUser);
    settings->setValue(Constant::DB_PASSWORD,databaseConfig.dbPass);
    settings->setValue(Constant::DB_PORT,databaseConfig.port);
    settings->endGroup();

    settings->beginGroup(Constant::SYSTEM_SETTING);
    settings->setValue(Constant::SYSTEM_DEFAULT_SCHEMES,systemConfigInfo.defaultKeySchemes);
    settings->setValue(Constant::SYSTEM_USER_SCHEMES_NAME,systemConfigInfo.userKeySchemesName);
    settings->setValue(Constant::SYSTEM_LOCALE,systemConfigInfo.locale);
    settings->setValue(Constant::SYSTEM_STYLE,systemConfigInfo.style);
    settings->setValue(Constant::SYSTEM_FULLSCREEN,systemConfigInfo.fullscreen);
    settings->setValue(Constant::SYSTEM_TOPHINT,systemConfigInfo.topHint);
    settings->setValue(Constant::SYSTEM_MENUBAR,systemConfigInfo.menubarVisible);
    settings->setValue(Constant::SYSTEM_LAYOUT,systemConfigInfo.layout);
    settings->setValue(Constant::SYSTEM_STATUSBAR,systemConfigInfo.m_bStatusBarVisible);
    settings->setValue(Constant::SYSTEM_NET_AUTOSTART,systemConfigInfo.m_bNetworkAutoCreateInStartProcess);
    settings->setValue(Constant::SYSTEM_NET_DEBUG,systemConfigInfo.m_bDebugNetParsedInfo);
    settings->setValue(Constant::SYSTEM_AUTO_START, systemConfigInfo.m_bAppAutoStart);
    settings->endGroup();
}

} //namespace Core
