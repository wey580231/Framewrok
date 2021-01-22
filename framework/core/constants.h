/*!
 *  @brief     常量汇总
 *  @details   定义了软件运行时所需的常量信息，包括(图片、控件ID等)
 *  @author    wey
 *  @version   1.0
 *  @date      2017.12.11
 *  @warning
 *  @copyright NanJing RenGu.
 */
#ifndef CONSTANTS_H
#define CONSTANTS_H

namespace Constant {

/*文件路径*/
const char AppShortName[] = "Rim";
const char ApplicationName[] = "ViewFrame";
const char Version[] = "1.1.0";

/*************************************************<配置文件config.ini>******************************************************/

/*系统设置*/
const char SYSTEM_SETTING[] = "System";
const char SYSTEM_DEFAULT_SCHEMES[] = "defaultSchemes";
const char SYSTEM_USER_SCHEMES_NAME[] = "userSchemesName";
const char SYSTEM_LOCALE[] = "locale";
const char SYSTEM_STYLE[] = "stylesheet";
const char SYSTEM_FULLSCREEN[] = "fullscreen";
const char SYSTEM_TOPHINT[] = "tophint";
const char SYSTEM_MENUBAR[] = "menubar";
const char SYSTEM_STATUSBAR[] = "statusbar";
const char SYSTEM_LAYOUT[] = "systemlayout";
const char SYSTEM_NET_AUTOSTART[] = "net_autostart";
const char SYSTEM_NET_DEBUG[] = "net_debug";
const char SYSTEM_AUTO_START[] = "autostart";

//日志模块
const char LOG_LOG[] = "log";
const char LOG_LOG_RECORDFILE[] = "log2File";
const char LOG_LOG_LEVEL[] = "logLevel";

//数据库模块
const char DB_SETTING[] = "Database";
const char DB_TYPE[] = "Dbtype";
const char DB_HOST[] = "Hostname";
const char DB_DATABASE_NAME[] = "DatabaseName";
const char DB_USERNAME[] = "UserName";
const char DB_PASSWORD[] = "Password";
const char DB_PORT[] = "Port";

/*人机海图设置*/
const char MAP_SETTING[] = "MapSetting";
const char MAP_USEMAPHTML[] = "useHtmlMap";
const char MAP_MAPPATH[] = "mapPath";
const char MAP_REFRESHINTERVAL[] = "refreshInterval";

//布局
const char LAY_SETTING[] = "Layout";
const char LAY_GEOMETRY[] = "geometry";
const char LAY_WINDOWSTATE[] = "windowState";

//状态栏
const char STATUSBAR_STATUSBAR[] = "StatusBar";


/*************************************************</配置文件>******************************************************/


/*************************************************<插件信息>******************************************************/
/*
 * 插件命名按照【<PLUGIN>.<插件所属模块>.<插件名>】
 * 设置插件所属模块便于系统将同一模块下的插件进行整合处理
*/
const char PLUGIN_CORE[] = "Plugin.Core.Core";

const char PLUGIN_TASK_CONTROL[] = "Plugin.TaskControl.TaskControl";

/*************************************************</插件信息>******************************************************/




/*************************************************<工具插件信息>******************************************************/
/*
 * 插件命名按照【<TOOL>.<插件名>】
 * 设置插件所属模块便于系统将同一模块下的插件进行整合处理
*/
const char TOOL_SCREEN_SHOT[] = "TOOL.Screenshot";
const char TOOL_DATABASE[] = "TOOL.DataBase";

/*************************************************</工具插件信息>******************************************************/



/*************************************************<工具栏>******************************************************/
//菜单栏
const char MENU_BAR[] = "MenuBar";

/*
 * 工具栏命名按照【<Menu>.<Group>.<工具栏名>】
*/
const char MENU_PROGRAM[]  = "Menu.Group.Program";
const char MENU_VIEW[]  = "Menu.Group.View";
const char MENU_SETTING[]  = "Menu.Group.Setting";
const char MENU_HELP[]     = "Menu.Group.Help";
const char DEFAULT_GROUP[] = "Menu.Default.Group";

/*
 * 菜单栏中action的Id命名按照【<Action>.<插件模块名>.<工具名>】
 * 请务必遵守，否则会影响快捷键(ShortcutSettings)的处理
*/

//MENU_PROGRAM 分组
const char PROGRAM_EXIT[] = "Action.Core.Exit";
const char SYSTEM_DELAY_SCREENSHOT[] = "Action.Core.DelayShot";
const char SYSTEM_NET_CONNECTION[] = "Action.Core.NetConnectionState";
const char SYSTEM_SYSTEM_SETTING[] = "Action.Core.SystemSetting";
const char SYSTEM_DATA_PLAYBACK[] = "Action.Core.DataPlayback";
const char SYSTEM_MAP_TOOLBAR[] = "Action.Core.MapToolBar";

//MENU_VIEW 分组
const char VIEW_MANAGER[] = "Action.Core.ViewManager";
const char IMPORT_VIEW[] = "Action.Core.ImportView";
const char EXPORT_VIEW[] = "Action.Core.ExportView";

//MENU_SETTING 分组
const char TOP_HINT[] = "Action.Core.TopHint";
const char FULL_SCREEN[] = "Action.Core.FullScreen";
const char CUSTOM_STYLE[] = "Action.Core.CustomStyle";
const char SYSTEM_LANGUAGE[] = "Action.Core.SystemLanguage";
const char SYSTEM_SHORTCUT[] = "Action.Core.ShortCut";
const char SYSTEM_SCREEN_SHOT[] = "Action.Core.ScreenShot";
const char SYSTEM_DATABASE[] = "Action.Core.DataBase";

//MENU_HELP 分组
const char TEC_SUPPORT[] = "Action.Core.TecSupport";
const char PLUGIN_DESC[] = "Action.Core.PluginDesc";
const char ABOUT_PROGRAM[] = "Action.Core.About";

/*************************************************</工具栏>******************************************************/

/*************************************************<状态栏>******************************************************/
const char STATUSBAR_SYSTEM[] = "StatusBar.System";
const char STATUSBAR_PLUGIN[] = "StatusBar.Plugin";

/*************************************************</状态栏>******************************************************/

}

#endif // CONSTANTS_H
