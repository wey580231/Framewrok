#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QAction>
#include <QActionGroup>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QDateTime>
#include <QScreen>
#include <QFileDialog>
#include <QTimer>
#include <QDesktopServices>
#include <QDateTimeEdit>

#include <base/selfwidget/rmessagebox.h>
#include <base/actionmanager/actioncontainer.h>
#include <base/actionmanager/actionmanager.h>
#include <base/actionmanager/action.h>
#include <base/actionmanager/statusbarcontainer.h>
#include <base/util/rsingleton.h>
#include <base/common/stylemanager.h>
#include <base/common/languagemanager.h>
#include <base/util/rlog.h>
#include <base/util/fileutils.h>
#include <base/util/rutil.h>

#include <core/constants.h>
#include <core/selfwidget/mydockwidget.h>
#include <core/selfwidget/mytabwidget.h>

#include <core/officalwidget/rdockwidget.h>
#include <core/officalwidget/rstatusbar.h>

#include <core/pluginmanager/layout/layoutmanager.h>
#include <core/pluginmanager/layout/officallayoutmanager.h>
#include <core/network/datarecord/binaryfilerecord.h>
#include <core/pluginmanager/mailbox/globalmailbox.h>

#include "../pluginmanager/interface/rcomponent.h"
#include "../pluginmanager/interface/rtoolcomponent.h"
#include "../pluginmanager/mailbox/subject.h"
#include "../pluginmanager/plugin/pluginmanager.h"
#include "../pluginmanager/plugin/pluginloader.h"

#include "../network/taskmanager.h"
#include "../network/dataparse/protocolparsethread.h"

#include "../protocol/protocolmanager.h"

#include "../global.h"
#include "../topiclist.h"
#include "../file/globalconfigfile.h"
#include "../file/programfilepath.h"
#include "../file/appconfig.h"
#include "shortcutsettings.h"
#include "modulesetting.h"
#include "titlebar.h"
#include "statusbar.h"
#include "pluginviewdialog.h"

#include <core/selfwidget/widget.h>

#include "flowcomponent/flowcomponentcontainer.h"
#include "statusbartool/datetimetool.h"
#include "statusbartool/globalstatusinfotool.h"
#include "statusbartool/netstatecontroltool.h"

#include "../file/appconfig.h"

using namespace Base;

namespace Core{

MainWindow * MainWindow::_mainWindow = NULL;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    titleBar(nullptr),m_statusBar(nullptr),settingBar(nullptr),layoutManager(nullptr),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    _mainWindow = this;
    RGlobal::G_GlobalConfigFile = nullptr;

    qRegisterMetaType<Datastruct::OriginalArray>("Datastruct::OriginalArray");
    qRegisterMetaType<Datastruct::PluginMessageList>("Datastruct::PluginMessageList");
    qRegisterMetaType<QImage>("QImage");
    qRegisterMetaType<Datastruct::NetworkState>("Datastruct::NetworkState");
    qRegisterMetaType<std::list<Datastruct::SystemStatusInfo>>("std::list<Datastruct::SystemStatusInfo>");

    RGlobal::G_WorkState.mWorkMode = Datastruct::SystemWorkState::WORK_RealData;
}

MainWindow *MainWindow::instance()
{
    return _mainWindow;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onMessage(QString messageType)
{
    if(messageType == QString(MessageType::MESS_LAN_CHANGED)){
        retranslateUi();
    }else if(messageType == QString(MessageType::MESS_NETWORK_CHANGED)){
        respNetworkUpdate();
    }
}

/*!
 * @brief 初始化所有的配置信息
 * @details 1.配置文件；2.创建各个路径；3.查找样式列表；4.加载可用的组件
 */
void MainWindow::initAll()
{
    ProgramFilePath programPath;

    //解析INI文件
    QSettings * settings = new QSettings(programPath.configFile,QSettings::IniFormat);
    RUtil::setGlobalSettings(settings);

    do{
#ifndef OFFICAL_DOCK
        layoutManager = new LayoutManager();
#endif
        if(nullptr == RGlobal::G_GlobalConfigFile){
            RGlobal::G_GlobalConfigFile = new GlobalConfigFile;
            RGlobal::G_GlobalConfigFile->setSettings(settings);

            if(!RGlobal::G_GlobalConfigFile->parseFile()){
                RMessageBox::warning(NULL,QObject::tr("Warning"),QObject::tr("System INI file parsed false!"),RMessageBox::Yes,RMessageBox::Yes);
                break;
            }
        }

        ProjectDesc::RegistAppToSystemStartUp(RGlobal::G_GlobalConfigFile->systemConfigInfo.m_bAppAutoStart);

        if(!RSingleton<RLog>::instance()->init(RGlobal::G_GlobalConfigFile->logConfig)){
            RMessageBox::warning(NULL,QObject::tr("Warning"),QObject::tr("Log module initialization failure!"),RMessageBox::Yes,RMessageBox::Yes);
            break;
        }

        if(RUtil::createDir(programPath.translationPath)){
            RSingleton<Base::LanguageManager>::instance()->loadTranslator(programPath.translationPath);
        }

        if(!RUtil::createDir(programPath.pluginConfigPath)){
            RMessageBox::warning(NULL,QObject::tr("Warning"),QObject::tr("Create plugin config path failed!"),RMessageBox::Yes,RMessageBox::Yes);
            break;
        }

        if(!RUtil::createDir(programPath.shortcutPath)){
            RMessageBox::warning(NULL,QObject::tr("Warning"),QObject::tr("Create schemes path failed!"),RMessageBox::Yes,RMessageBox::Yes);
            break;
        }

        if(!RUtil::createDir(programPath.recordPath)){
            RMessageBox::warning(NULL,QObject::tr("Warning"),QObject::tr("Create record path failed!"),RMessageBox::Yes,RMessageBox::Yes);
            break;
        }

        if(!RUtil::createDir(programPath.layoutPath)){
            RMessageBox::warning(NULL,QObject::tr("Warning"),QObject::tr("Create layouts path failed!"),RMessageBox::Yes,RMessageBox::Yes);
            break;
        }

		//注册内置样式表
        RSingleton<StyleManager>::instance()->addStyle(new CustomStyle(tr("Technology style"),":/resource/style/Technology.qss",true));
        RSingleton<StyleManager>::instance()->addStyle(new CustomStyle(tr("Dark style"),":/resource/style/Black.qss",false));
        RSingleton<StyleManager>::instance()->addStyle(new CustomStyle(tr("Light style"),":/resource/style/White.qss",false));
        RSingleton<StyleManager>::instance()->addStyle(new CustomStyle(tr("QYBlue style"),":/resource/style/QYBlue.qss",false));

		//注册动态解析样式表
        RSingleton<StyleManager>::instance()->dynamicLoadStyle(programPath.stylePath);

        Subject::instance()->attach(this);

        initMenu();
        initComponent();

        updateLanguage(curLanguageName);

//        setWindowIcon(QIcon(":/tech/resource/technology/logo1.jpg"));

        loadUserSetting();

    }while(0);
}

void MainWindow::initMenu()
{
    ActionContainer *menubar = ActionManager::instance()->createMenuBar(Constant::MENU_BAR);

    setMenuBar(menubar->menuBar());

    menubar->appendGroup(Constant::MENU_PROGRAM);
    menubar->appendGroup(Constant::MENU_VIEW);
    menubar->appendGroup(Constant::MENU_SETTING);
    menubar->appendGroup(Constant::MENU_HELP);

    QAction * coreAction = new QAction();
    coreAction->setVisible(false);
    ActionManager::instance()->registAction(Constant::PLUGIN_CORE,coreAction);

    //程序菜单
    serverMenu = ActionManager::instance()->createMenu(Constant::MENU_PROGRAM);
    serverMenu->appendGroup(Constant::MENU_PROGRAM);
    menubar->addMenu(serverMenu, Constant::MENU_PROGRAM);

    exitAction = new QAction(this);
    connect(exitAction,SIGNAL(triggered()),this,SLOT(programExit()));
    Action * serverDelete = ActionManager::instance()->registAction(Constant::PROGRAM_EXIT,exitAction);
    serverDelete->setDefaultKey(QKeySequence("Ctrl+Q"));

    serverMenu->addAction(serverDelete,Constant::MENU_PROGRAM);

    //视图菜单
    viewMenu = ActionManager::instance()->createMenu(Constant::MENU_VIEW);
    viewMenu->appendGroup(Constant::MENU_VIEW);
//    menubar->addMenu(viewMenu, Constant::MENU_VIEW);

    viewManagerMenu = ActionManager::instance()->createMenu(Constant::VIEW_MANAGER);
    viewManagerMenu->appendGroup(Constant::VIEW_MANAGER);
    viewMenu->addMenu(viewManagerMenu,Constant::VIEW_MANAGER);

#ifdef OFFICAL_DOCK
    QActionGroup * layoutGroup = new QActionGroup(this);
    QList<QString> t_registLayouts = OfficalLayoutManager::instance()->registedLayouts();
    int t_i_layoutLoopCount = 0;
    std::for_each(t_registLayouts.begin(),t_registLayouts.end(),[&](QString layoutName){
        QAction * action = new QAction(layoutName);
        OfficalLayoutManager::instance()->bindActionToLayout(layoutName,action);
        action->setCheckable(true);

        layoutGroup->addAction(action);
        connect(action,SIGNAL(triggered()),this,SLOT(switchLayout()));
        action->setProperty(Constant::VIEW_MANAGER,t_i_layoutLoopCount);
        Action * tmpStyle = ActionManager::instance()->registAction(QString("LayoutAction%1").arg(t_i_layoutLoopCount).toLocal8Bit().data(),action);
        viewManagerMenu->addAction(tmpStyle,Constant::VIEW_MANAGER);

        t_i_layoutLoopCount++;
    });
#else
    //20190328 查找默认布局路径下已有的布局
    QStringList t_exsitedLayouts = layoutManager->localLayoutFilenames();
    if(layoutManager->existed(RGlobal::G_GlobalConfigFile->systemConfigInfo.layout))
        layoutManager->setCurrentLayout(RGlobal::G_GlobalConfigFile->systemConfigInfo.layout);
    else
        layoutManager->resotreDefaultLayout();

    QActionGroup * layoutGroup = new QActionGroup(this);
    int t_i_layoutLoopCount = 0;
    std::for_each(t_exsitedLayouts.begin(),t_exsitedLayouts.end(),[&](QString fileName){
        QAction * action = new QAction(fileName);
        action->setCheckable(true);

        if(layoutManager->getCurrentLayout() == fileName){
            action->setChecked(true);
        }

        layoutGroup->addAction(action);
        connect(action,SIGNAL(triggered()),this,SLOT(switchLayout()));
        action->setProperty(Constant::VIEW_MANAGER,t_i_layoutLoopCount);
        Action * tmpStyle = ActionManager::instance()->registAction(QString("LayoutAction%1").arg(t_i_layoutLoopCount).toLocal8Bit().data(),action);
        viewManagerMenu->addAction(tmpStyle,Constant::VIEW_MANAGER);

        t_i_layoutLoopCount++;
    });

    if(t_i_layoutLoopCount > 0)
        viewManagerMenu->addSeparator(Constant::VIEW_MANAGER);

    importViewAction = new QAction(this);
    connect(importViewAction,SIGNAL(triggered()),layoutManager,SLOT(importView()));
    Action * importView = ActionManager::instance()->registAction(Constant::IMPORT_VIEW,importViewAction);
    importView->setDefaultKey(QKeySequence("Ctrl+Shift+I"));
    viewManagerMenu->addAction(importView,Constant::VIEW_MANAGER);

    exportViewAction = new QAction(this);
    connect(exportViewAction,SIGNAL(triggered(bool)),layoutManager,SLOT(exportView(bool)));
    Action * exportView = ActionManager::instance()->registAction(Constant::EXPORT_VIEW,exportViewAction);
    exportView->setDefaultKey(QKeySequence("Ctrl+Shift+E"));
    viewManagerMenu->addAction(exportView,Constant::VIEW_MANAGER);
#endif

    viewMenu->addMenu(viewManagerMenu,Constant::MENU_VIEW);
    viewMenu->addSeparator(Constant::MENU_VIEW);

    //设置菜单
    settingsMenu = ActionManager::instance()->createMenu(Constant::MENU_SETTING);
    settingsMenu->appendGroup(Constant::MENU_SETTING);
    menubar->addMenu(settingsMenu, Constant::MENU_SETTING);

    topHintAction = new QAction(this);
    topHintAction->setCheckable(true);
    connect(topHintAction,SIGNAL(triggered(bool)),this,SLOT(windowTopHint(bool)));
    Action * topHint = ActionManager::instance()->registAction(Constant::TOP_HINT,topHintAction);
    topHint->setDefaultKey(QKeySequence("Ctrl+Shift+T"));
    settingsMenu->addAction(topHint,Constant::MENU_SETTING);

    fullScreenAction = new QAction(this);
    fullScreenAction->setCheckable(true);
    connect(fullScreenAction,SIGNAL(triggered(bool)),this,SLOT(windowFullScreen(bool)));
    Action * fullScreen = ActionManager::instance()->registAction(Constant::FULL_SCREEN,fullScreenAction);
    fullScreen->setDefaultKey(QKeySequence("Ctrl+Shift+f11"));
    settingsMenu->addAction(fullScreen,Constant::MENU_SETTING);

    settingsMenu->addSeparator(Constant::MENU_SETTING);

    //样式
    settingsMenu->appendGroup(Constant::CUSTOM_STYLE);
    styleMenu = ActionManager::instance()->createMenu(Constant::CUSTOM_STYLE);
    styleMenu->appendGroup(Constant::CUSTOM_STYLE);
    settingsMenu->addMenu(styleMenu,Constant::CUSTOM_STYLE);

    StylePtrList styles = RSingleton<StyleManager>::instance()->styles();
    QActionGroup * styleGroup = new QActionGroup(this);
    for(int i = 0;i<styles.size();i++){
        QAction * action = new QAction(styles.at(i)->getStyleName());
        action->setCheckable(true);
        if(styles.at(i)->isSelected()){
            action->setChecked(styles.at(i)->isSelected());
            updateStyle(i);
        }
        styles.at(i)->setAction(action);
        styleGroup->addAction(action);
        connect(action,SIGNAL(triggered()),this,SLOT(switchStyle()));
        action->setProperty(Constant::CUSTOM_STYLE,i);
        Action * tmpStyle = ActionManager::instance()->registAction(QString("StyleAction%1").arg(i).toLocal8Bit().data(),action);
        styleMenu->addAction(tmpStyle,Constant::CUSTOM_STYLE);
    }

    //翻译文件
    settingsMenu->appendGroup(Constant::SYSTEM_LANGUAGE);
    lanMenu = ActionManager::instance()->createMenu(Constant::SYSTEM_LANGUAGE);
    lanMenu->appendGroup(Constant::SYSTEM_LANGUAGE);
//    settingsMenu->addMenu(lanMenu,Constant::SYSTEM_LANGUAGE);

    LanguagePtrList lans = RSingleton<LanguageManager>::instance()->languages();
    QActionGroup * lanGroup = new QActionGroup(this);
    for(int i = 0;i<lans.size();i++){
        QAction * action = new QAction(lans.at(i)->getName());
        action->setCheckable(true);
        //隐藏中英文切换
        action->setVisible(false);

        if(lans.at(i)->isSelected()){
            action->setChecked(lans.at(i)->isSelected());
            curLanguageName = lans.at(i)->getFileName();
        }
        lans.at(i)->setAction(action);
        lanGroup->addAction(action);
        connect(action,SIGNAL(triggered()),this,SLOT(switchLanguage()));
        action->setProperty(Constant::SYSTEM_LANGUAGE,lans.at(i)->getFileName());
        Action * tmpStyle = ActionManager::instance()->registAction(QString("LanAction%1").arg(i).toLocal8Bit().data(),action);
        lanMenu->addAction(tmpStyle,Constant::SYSTEM_LANGUAGE);
    }

    //快捷键盘
    settingsMenu->appendGroup(Constant::SYSTEM_SHORTCUT);
    shortcutAction = new QAction(this);
    connect(shortcutAction,SIGNAL(triggered()),this,SLOT(showShortcutSettings()));
    Action * shortcut = ActionManager::instance()->registAction(Constant::SYSTEM_SHORTCUT,shortcutAction);
    shortcut->setDefaultKey(QKeySequence("Ctrl+Shift+K"));
    //隐藏快捷键
    shortcutAction->setVisible(false);

    settingsMenu->addAction(shortcut,Constant::SYSTEM_SHORTCUT);

    //帮助菜单
    helpMenu = ActionManager::instance()->createMenu(Constant::MENU_HELP);
    helpMenu->appendGroup(Constant::MENU_HELP);
    menubar->addMenu(helpMenu, Constant::MENU_HELP);

    supportAction = new QAction(this);
    connect(supportAction,SIGNAL(triggered()),this,SLOT(technicalSupport()));
    Action * support = ActionManager::instance()->registAction(Constant::TEC_SUPPORT,supportAction);
    helpMenu->addAction(support,Constant::MENU_HELP);

    pluginSpecAction = new QAction(this);
    connect(pluginSpecAction,SIGNAL(triggered()),this,SLOT(showPluginDesc()));
    Action * pluginSpec = ActionManager::instance()->registAction(Constant::PLUGIN_DESC,pluginSpecAction);
    helpMenu->addAction(pluginSpec,Constant::MENU_HELP);

    helpMenu->addSeparator(Constant::MENU_HELP);

    aboutPorgramAction = new QAction(this);
    connect(aboutPorgramAction,SIGNAL(triggered()),this,SLOT(aboutProgram()));
    Action * aboutProgram = ActionManager::instance()->registAction(Constant::ABOUT_PROGRAM,aboutPorgramAction);
    helpMenu->addAction(aboutProgram,Constant::MENU_HELP);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(programExit())
        event->accept();
    else
        event->ignore();
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
#ifndef OFFICAL_DOCK
    if(m_statusBar){
        m_statusBar->fixedPosBottom();
    }
#endif
}

/*!
 * @brief   程序退出
 */
bool MainWindow::programExit()
{
    int result = RMessageBox::information(this,QStringLiteral("提示"),QStringLiteral("是否退出程序?"),RMessageBox::Yes|RMessageBox::No);
    if(result == RMessageBox::No)
        return false;

#ifdef OFFICAL_DOCK
    OfficalLayoutManager::instance()->exportView();
#else
    layoutManager->exportView(true);
#endif

    RSingleton<ProtocolParseThread>::instance()->stopMe();

    Core::TaskManager::instance()->releaseTask();

    PluginManager::instance()->releaseALL();

    if(m_statusBar){
        delete m_statusBar;
    }

    exit(0);

    return true;
}

/*!
 * @brief 手动切换布局，config.ini中同步更新布局信息
 * @attention 在使用OFFICAL_DOCK模式时，切换布局后，会存在窗口超出屏幕的问题，需要手动触发窗口大小的变化。
 */
void MainWindow::switchLayout()
{
    QAction * t_action = qobject_cast<QAction *>(QObject::sender());
    if(t_action){
#ifdef OFFICAL_DOCK
        OfficalLayoutManager::instance()->switchLayout(t_action->text());
        showNormal();
        showMaximized();

        RGlobal::G_GlobalConfigFile->systemConfigInfo.layout = t_action->text();
        RGlobal::G_GlobalConfigFile->saveFile();
#else
        ProgramFilePath filePath;
        QString t_sz_layoutFullPath = filePath.layoutPath + QDir::separator() + t_action->text() /*+ ".bin"*/;
        if(layoutManager->importView(t_sz_layoutFullPath)){
            layoutManager->setCurrentLayout(t_action->text());
            RGlobal::G_GlobalConfigFile->systemConfigInfo.layout = t_action->text();
            RGlobal::G_GlobalConfigFile->saveFile();
        }
#endif
    }
}

/*!
 * @brief   程序置顶
 */
void MainWindow::windowTopHint(bool flag)
{
    Qt::WindowFlags flags = windowFlags();
    if(flag)
        flags |= Qt::WindowStaysOnTopHint;
    else
        flags = flags & ~Qt::WindowStaysOnTopHint;

    setWindowFlags(flags);
    //取消置顶时需要判断窗口是否隐藏了，如果隐藏则将它显示出来
    if(!this->isVisible())
        setVisible(true);

    RGlobal::G_GlobalConfigFile->systemConfigInfo.topHint = flag;
    RGlobal::G_GlobalConfigFile->saveFile();
}

void MainWindow::windowFullScreen(bool flag)
{
    Qt::WindowStates state = windowState();

    RGlobal::G_GlobalConfigFile->systemConfigInfo.fullscreen = flag;
    if(flag)
    {
        state |= Qt::WindowFullScreen;
        setWindowState(state);
    }
    else
    {
        state = state & ~Qt::WindowFullScreen;
        setWindowState(state);
        showMaximized();
    }

    RGlobal::G_GlobalConfigFile->saveFile();
}

/*!
 * @brief 切换显示样式
 */
void MainWindow::switchStyle()
{
    int switchIndex = QObject::sender()->property(Constant::CUSTOM_STYLE).toInt();
    StylePtrList list = RSingleton<StyleManager>::instance()->styles();
    if(switchIndex >= 0 && switchIndex < list.size()){
        updateStyle(switchIndex);
        RGlobal::G_GlobalConfigFile->systemConfigInfo.style = RSingleton<StyleManager>::instance()->currentStyle()->getStyleName();
        RGlobal::G_GlobalConfigFile->saveFile();
    }
}

void MainWindow::updateStyle(int index)
{
    RSingleton<StyleManager>::instance()->switchStyle(index);
}

/*!
 *  @brief 切换显示语言
 */
void MainWindow::switchLanguage()
{
    QString switchLan = QObject::sender()->property(Constant::SYSTEM_LANGUAGE).toString();
    updateLanguage(switchLan);
    RGlobal::G_GlobalConfigFile->systemConfigInfo.locale = switchLan;
    RGlobal::G_GlobalConfigFile->saveFile();
}

/*!
 * @brief 根据翻译文件名切换文件
 * @param[in]  lanFileName 要切换的文件名
 */
void MainWindow::updateLanguage(QString lanFileName)
{
    if(RSingleton<LanguageManager>::instance()->switchLanguage(lanFileName)){
        Subject::instance()->notify(MessageType::MESS_LAN_CHANGED);

//TODO 20190401未将信息通知至各个子插件
//        Datastruct::PluginMessage * test = new Datastruct::PluginMessage;
//        test->message.m_broadCast = true;
//        test->message.topicId = MessageType::MESS_LAN_CHANGED;

//        GlobalMailBox::instance()->postSystemMessage(test);
    }
}

/*!
 * @brief 加载上一次的默认配置
 * @details 1.语言设置； @n
 *          2.样式表设置； @n
 *          3.从config/config.ini文件中查找是否有快捷键设置，若存在快捷键设置则使用 @n
 *          4.窗口设置(20181113) @n
 *          5.布局设置(待添加) @n
 */
void MainWindow::loadUserSetting()
{
    //[1] 语言设置
    if(RGlobal::G_GlobalConfigFile->systemConfigInfo.locale.isEmpty()){
        RGlobal::G_GlobalConfigFile->systemConfigInfo.locale = curLanguageName;
        RGlobal::G_GlobalConfigFile->saveFile();
    }else{
        Base::Language * currLan = RSingleton<LanguageManager>::instance()->findLan(RGlobal::G_GlobalConfigFile->systemConfigInfo.locale);
        if(currLan){
            currLan->getAction()->setChecked(true);
            updateLanguage(RGlobal::G_GlobalConfigFile->systemConfigInfo.locale);
        }
    }

    //[2] 样式表设置
    if(RGlobal::G_GlobalConfigFile->systemConfigInfo.style.isEmpty()){
        RGlobal::G_GlobalConfigFile->systemConfigInfo.style = RSingleton<StyleManager>::instance()->currentStyle()->getStyleName();
        RGlobal::G_GlobalConfigFile->saveFile();
    }else{
        Base::CustomStyle * currStyle = RSingleton<StyleManager>::instance()->findStyle(RGlobal::G_GlobalConfigFile->systemConfigInfo.style);
        if(currStyle){
            currStyle->getAction()->setChecked(true);
            updateStyle(currStyle->getAction()->property(Constant::CUSTOM_STYLE).toInt());
        }
    }

    //[3] 快捷键
//    if(!RGlobal::G_GlobalConfigFile->systemConfigInfo.defaultKeySchemes){
//        ProgramFilePath filePath;
//        RXmlFile xmlfile(filePath.shortcutPath + QDir::separator() + RGlobal::G_GlobalConfigFile->systemConfigInfo.userKeySchemesName);
//        ShortcutParseMethod * method = new ShortcutParseMethod();
//        xmlfile.setParseMethod(method);
//        if(xmlfile.startParse(QFile::ReadOnly)){
//            ShortcutParseMethod::ShortcutMappingList list = method->getParsedList();
//            ActionManager::ActionMap actMap = ActionManager::instance()->getAllActions();

//            ActionManager::ActionMapIterator iter = actMap.constBegin();
//            while(iter != actMap.constEnd()){
//                QString id = const_cast<Id&>(iter.key()).data();
//                if(id.indexOf(".") > 0 && id.split(".").size() == 2){
//                    auto findIndex = std::find_if(list.constBegin(),list.constEnd(),[&id](const ShortcutMapping & item){
//                        return item.id == id;
//                    });
//                    if(findIndex != list.constEnd()){
//                        iter.value()->setDefaultKey(QKeySequence((*findIndex).keySequence));
//                    }else{
//                        iter.value()->setDefaultKey(QKeySequence());
//                    }
//                }
//                iter++;
//            }
//        }else{
//            RMessageBox::warning(this,tr("warning"),tr("User defined shortcut keys not available!"),RMessageBox::Yes);
//        }
//    }

    //[4]窗口设置
    windowTopHint(RGlobal::G_GlobalConfigFile->systemConfigInfo.topHint);
    ActionManager::instance()->action(Constant::TOP_HINT)->action()->setChecked(RGlobal::G_GlobalConfigFile->systemConfigInfo.topHint);

    windowFullScreen(RGlobal::G_GlobalConfigFile->systemConfigInfo.fullscreen);
    ActionManager::instance()->action(Constant::FULL_SCREEN)->action()->setChecked(RGlobal::G_GlobalConfigFile->systemConfigInfo.fullscreen);

    //[5]布局
}

void MainWindow::retranslateUi()
{
    setWindowTitle(ProjectDesc::GetProjectDesc());

    serverMenu->menu()->setTitle(tr("Program(&P)"));
    exitAction->setText(tr("Exit(&X)"));

    viewMenu->menu()->setTitle(tr("View(&V)"));
    viewManagerMenu->menu()->setTitle(tr("View manager"));
#ifndef OFFICAL_DOCK
    importViewAction->setText(tr("Import view"));
    exportViewAction->setText(tr("Export view"));
#endif

    settingsMenu->menu()->setTitle(tr("Settings(&S)"));
    topHintAction->setText(tr("Top hint"));
    fullScreenAction->setText(tr("Full screen"));

    styleMenu->menu()->setTitle(tr("Styles(&Y)"));
    lanMenu->menu()->setTitle(tr("Language(&L)"));
    shortcutAction->setText(tr("Shortcut settings(&T)"));

    helpMenu->menu()->setTitle(tr("Help(&H)"));
    supportAction->setText(tr("Technical support(&T)"));
    pluginSpecAction->setText(tr("About plugins(&p)"));
    aboutPorgramAction->setText(tr("About program(&A)"));

    //WARNING 20190327 需手动对插件所属组信息翻译
    m_pluginGroupName.clear();

    m_pluginGroupName.insert("Plugin.TaskControl",tr("Task Control"));
    m_pluginGroupName.insert("Plugin.HealthManager",tr("Health Manager"));
    m_pluginGroupName.insert("Plugin.DataDisplay",tr("Data Display"));
    m_pluginGroupName.insert("Plugin.AutoTest",tr("Auto Test"));

    PluginManager::ActiveComponentMap maps = PluginManager::instance()->getAllActivePlugins();
    PluginManager::ActiveComponentMap::iterator iter = maps.begin();
    while(iter != maps.end()){
        Id id = iter.value().pComp->id();
        Action * act = ActionManager::instance()->action(id);
        if(act){
            act->action()->setText(iter.value().pComp->name());
        }

        QStringList slist = id.toString().split(".");
        QString menuId = slist.at(0)+"."+slist.at(1);
        char mid[128] = {0};
        memcpy(mid,menuId.toLocal8Bit().data(),menuId.toLocal8Bit().size());
        Action * module = ActionManager::instance()->action(mid);
        if(module){
            module->action()->setText(m_pluginGroupName.value(QString(mid)));
        }
        iter++;
    }
}

/*!
 * @brief 系统网络接口更新，同步更新网路记录策略
 */
void MainWindow::respNetworkUpdate()
{
    NetworkMap * maps = PluginLoader::instance()->getNetworks();
    RSingleton<ProtocolParseThread>::instance()->getFileRecord()->updateNetworkInterface(maps);
}

void MainWindow::technicalSupport()
{

}

void MainWindow::aboutProgram()
{

}

/*!
 * @brief 显示插件所加载的插件信息
 */
void MainWindow::showPluginDesc()
{
    PluginViewDialog dialog(this);
    dialog.exec();
}

/*!
 * @brief 显示快捷键设置窗口
 * @details 初始显示所有快捷键信息，按照插件分类显示；
 */
void MainWindow::showShortcutSettings()
{
    ShortcutSettings ss(this);
    ss.exec();
}

/*!
 * @brief   初始化各个组件
 * @details 1.初始dockewidget
 *          2.解析插件配置文件
 *          3.初始化状态栏
 *          4.初始化业务插件
 *          5.初始化网络连接
 *          6.初始化工具插件
 */
void MainWindow::initComponent()
{
    setDockNestingEnabled(true);

    //[1]
    if(!RGlobal::G_GlobalConfigFile->systemConfigInfo.menubarVisible){
        menuBar()->hide();
    }

#ifdef OFFICAL_DOCK

#ifdef ENABLE_BUSINESS_PLUGIN
    setCentralWidget(nullptr);
#else
    //NOTE 非插件模式下,可自己设置centralWidget

#endif

#else
    QHBoxLayout * mainLayout = new QHBoxLayout;
    mainLayout->setContentsMargins(0,0,0,0);
    centralWidget()->setLayout(mainLayout);
#endif

    //连接全局信箱至框架
    connect(GlobalMailBox::instance(),SIGNAL(sendToSystem(Datastruct::PluginMessage*)),this,SLOT(processToSystemMessage(Datastruct::PluginMessage*)));

    //[2]
    if(!PluginLoader::instance()->loadPluginXML()){
        RMessageBox::warning(0,QStringLiteral("警告"),QStringLiteral("解析PluginResp.xml文件错误,程序退出!"),RMessageBox::Yes);
        exit(-1);
    }

    if(!PluginLoader::instance()->loadModuleFile()){
        RMessageBox::warning(0,QStringLiteral("警告"),QStringLiteral("解析Plugin.xml文件错误,程序退出!"),RMessageBox::Yes);
        exit(-1);
    }

    //[3]
    initStatusBar();

    //只有在定义了'ENABLE_BUSINESS_PLUGIN'或'ENABLE_TOOL_PLUGIN'宏之后才会加载对应的dll
    PluginManager::instance()->load();

    //[4]
#ifdef ENABLE_BUSINESS_PLUGIN
    initBusinessPlugin();
#endif

    //[5]
    initNetwork();

    //[6]
#ifdef ENABLE_TOOL_PLUGIN
    initToolPlugin();
#endif

#ifndef OFFICAL_DOCK
    //初始化控制面板、标题栏
    settingBar = new ModuleSetting(this);
    PluginManager::instance()->addNormalWidget(settingBar);
    connect(settingBar,SIGNAL(raiseWidget(QString)),PluginManager::instance(),SLOT(rasieWidget(QString)));

    QRect screenGeometry = RUtil::screenGeometry();
    int settingBarWidth = 265;
    int settingBarHeight = 620;
    settingBar->setMinimumSize(settingBarWidth,settingBarHeight);
    settingBar->setGeometry(screenGeometry.width() - settingBarWidth,160,settingBarWidth,settingBarHeight);

    if(!RGlobal::G_GlobalConfigFile->systemConfigInfo.menubarVisible){
        titleBar = new TitleBar(this);
        PluginManager::instance()->addNormalWidget(titleBar);
        int minTitileBarHeight = 80;
        titleBar->setMinimumHeight(minTitileBarHeight);
        titleBar->setGeometry(0,0,screenGeometry.width(),minTitileBarHeight);
        titleBar->addMenu(QStringLiteral("图层(&P)"),QString(Constant::MENU_VIEW),QPixmap(":/tech/resource/technology/system_setting.png"));
        titleBar->addMenu(QStringLiteral("工具(&T)"),QString(Constant::MENU_PROGRAM),QPixmap(":/tech/resource/technology/system_setting.png"));
        titleBar->addMenu(QStringLiteral("设置(&S)"),QString(Constant::MENU_SETTING),QPixmap(":/tech/resource/technology/system_setting.png"));
        titleBar->addMenu(QStringLiteral("帮助(&H)"),QString(Constant::MENU_HELP),QPixmap(":/tech/resource/technology/system_setting.png"));
    }
#endif
}

/*!
 * @brief 初始状态栏
 */
void MainWindow::initStatusBar()
{
    //创建悬浮显示容器(需在状态窗口前)
    FlowComponentContainer * t_flowComponent = new FlowComponentContainer(this);

    //状态窗口需在各个插件实例化前准备好
    if(RGlobal::G_GlobalConfigFile->systemConfigInfo.m_bStatusBarVisible){
        m_statusBar = new StatusBar(this);
        m_statusBar->fixedPosBottom();

        Id systemId(Constant::STATUSBAR_SYSTEM);
        Id pluginId(Constant::STATUSBAR_PLUGIN);

        m_statusBar->addContainer(pluginId);
        m_statusBar->addContainer(systemId);

		Base::StatusBarContainer * t_defaultContainer = m_statusBar->container(systemId);
        m_statusBar->appendItem(systemId,(new NetStateControlTool(m_statusBar))->getItemInfo());
        t_defaultContainer->addStretch();
        m_statusBar->appendItem(systemId,(new GlobalStatusInfoTool(m_statusBar))->getItemInfo());
        m_statusBar->appendItem(systemId,(new DatetimeTool(m_statusBar))->getItemInfo());

        m_statusBar->loadLocalSetting();

#ifdef OFFICAL_DOCK
        RStatusBar * rstatusBar = new RStatusBar(this);
        rstatusBar->addPermanentWidget(m_statusBar);
        rstatusBar->setVisible(RGlobal::G_GlobalConfigFile->systemConfigInfo.m_bStatusBarVisible);
        setStatusBar(rstatusBar);
#else
        m_statusBar->setVisible(RGlobal::G_GlobalConfigFile->systemConfigInfo.m_bStatusBarVisible);
#endif
    }
}

/*!
 * @brief 初始化网络
 */
void MainWindow::initNetwork()
{
    if(RGlobal::G_GlobalConfigFile->systemConfigInfo.m_bNetworkAutoCreateInStartProcess){
        NetworkMap * networkMap = PluginLoader::instance()->getNetworks();
        NetworkMap::iterator niter = networkMap->begin();
        while(niter != networkMap->end()){
            createNetwork(niter.value());
            niter++;
        }
    }

    //【3.1】网络解析
    RSingleton<ProtocolParseThread>::instance();
    connect(RSingleton<ProtocolParseThread>::instance(),SIGNAL(protocolDataReady()),this,SLOT(dispatchProtocolData())/*,Qt::BlockingQueuedConnection*/);

    RSingleton<ProtocolParseThread>::instance()->startMe();         // 协议解析线程
}

/*!
 * @brief 初始业务插件
 */
void MainWindow::initBusinessPlugin()
{
    QMap<QString,QStringList> topicPluginMap;            /*!< 20190327增加记录插件发布与订阅主题集合 key:主题号,value:订阅此主题的插件ID */
    QMap<QString,QStringList> msgSystemMap;              /*!< 20190929增加插件订阅系统信息集合 key:主题号，value:订阅此主题的插件ID */

    //【2】实例化模块
    QStringList t_avaliablePluginList;
    ModuleList * moduleList = PluginLoader::instance()->getModules();
    ModuleList::iterator miter = moduleList->begin();
    while(miter != moduleList->end()){
        Datastruct::ModuleInfo mm = *miter;

        RComponent * plugin = PluginManager::instance()->getAvailblePlugin(mm.pluginId);

        if(plugin){
            plugin = plugin->clone();

            //20190327记录所有插件发布的主题
            QStringList pubTopicList = plugin->publishTopic();
            std::for_each(pubTopicList.begin(),pubTopicList.end(),[&topicPluginMap](QString topic){
                if(!topicPluginMap.contains(topic))
                    topicPluginMap.insert(topic,QStringList());
            });

            plugin->setModuleInfo(mm);
            plugin->setName(mm.name);
            t_avaliablePluginList<<mm.name;
#ifdef OFFICAL_DOCK
            RDockWidget * dockContainer = new RDockWidget(this);
            QStringList t_idList = plugin->id().toString().split(".");
            if(t_idList.size() == 3){
                QString t_groupId = t_idList.at(1);
                addDockWidget(t_groupId,t_idList.at(2),Qt::RightDockWidgetArea,dockContainer);
            }
            plugin->setDockContainer(dockContainer);
#else
            MyDockWidget * dockContainer = new MyDockWidget(this);
            dockContainer->setGeometry(mm.geometry);
            plugin->setDockContainer(dockContainer);
#endif

            PluginManager::instance()->addActivePlugin(plugin,mm);
        }
        miter++;
    }

    RLOG_INFO("avaliable plugin [%s]",t_avaliablePluginList.join(",").toLocal8Bit().data());

    PluginManager::ActiveComponentMap & maps = PluginManager::instance()->getAllActivePlugins();

    //【3】为插件创建显示窗口、注册网络协议
    PluginManager::ActiveComponentMap::iterator iter = maps.begin();
    while(iter != maps.end()){
        /*! <通用组件>  */
        RComponent * comp = iter.value().pComp;

#ifdef OFFICAL_DOCK
        RDockWidget * dockContainer = comp->getDockContainer();
        QWidget * widget = comp->initialize(this);
#else
        MyDockWidget * dockContainer = comp->getDockContainer();
        dockContainer->setFloating(true);
        QWidget * widget = comp->initialize(dockContainer);
#endif

        if(widget)
        {
            dockContainer->setWidget(widget);
            dockContainer->setObjectName(comp->objectName());
            dockContainer->setWindowTitle(comp->name());

            //对所有插件进行协议信息统计
            ProtocolDataStaticInfoList tlist = comp->getStaticInfoList();
            std::for_each(tlist.begin(),tlist.end(),[&](ProtocolDataStaticInfo info){
                if(RGlobal::G_NetCardStaticMap.contains(info.m_netId)){
                    NetCardStaticInfo & t_cardInfo = RGlobal::G_NetCardStaticMap.operator[](info.m_netId);
                    auto iter = std::find_if(t_cardInfo.m_protocolStaticsInfo.begin(),t_cardInfo.m_protocolStaticsInfo.end(),[&](ProtocolDataStaticInfo & tmpInfo){
                        return tmpInfo.m_name == info.m_name && tmpInfo.m_iType == info.m_iType;
                    });

                    if(iter == t_cardInfo.m_protocolStaticsInfo.end()){
                        t_cardInfo.m_protocolStaticsInfo.insert(info.m_iType,info);
                    }
                }else{
                    NetCardStaticInfo t_cardInfo;
                    t_cardInfo.m_ip = info.m_ip;
                    t_cardInfo.m_networkId = info.m_netId;
                    t_cardInfo.m_protocolStaticsInfo.insert(info.m_iType,info);
                    RGlobal::G_NetCardStaticMap.insert(info.m_netId,t_cardInfo);
                }
            });

#ifndef OFFICAL_DOCK
            connect(dockContainer,SIGNAL(groupDockWidget(QString,QPoint)),PluginManager::instance(),SLOT(groupDockWidget(QString,QPoint)));
            connect(dockContainer,SIGNAL(dragStateChanged(Datastruct::DockDragState)),PluginManager::instance(),SLOT(respDragState(Datastruct::DockDragState)));
#endif
            //更新发布主题的订阅者
            QStringList subTopicList = comp->subscribeTopic();
            std::for_each(subTopicList.begin(),subTopicList.end(),[&topicPluginMap,&comp](QString topic){
                if(topicPluginMap.contains(topic) && !topicPluginMap[topic].contains(comp->id().toString())){
                    topicPluginMap.operator [](topic)<<comp->id().toString();
                }
            });

            //更新系统主题信息的订阅者
            QStringList subSystemList = comp->subscribeSystemMsg();
            std::for_each(subSystemList.begin(),subSystemList.end(),[&msgSystemMap,&comp](QString topic){
                if(msgSystemMap.contains(topic)){
                    if(!msgSystemMap[topic].contains(comp->id().toString()))
                        msgSystemMap.operator [](topic)<<comp->id().toString();
                }else{
                    QStringList tmplist;
                    tmplist<<comp->id().toString();
                    msgSystemMap.insert(topic,tmplist);
                }
            });
        }else{

#ifdef OFFICAL_DOCK
            QStringList t_idList = comp->id().toString().split(".");
            if(t_idList.size() == 3){
                QString t_groupId = t_idList.at(1);
                removeDockWidget(t_groupId,t_idList.at(2));
            }
#endif

            RLOG_ERROR("initialize module[%s]error",comp->pluginName().toLocal8Bit().data());
            delete comp->getDockContainer();
            iter = maps.erase(iter);
            continue;
        }

#ifndef OFFICAL_DOCK
        //将dock中控制显隐的action添加至菜单栏
        if(dockContainer->toggleViewAction()){
            QStringList slist = comp->id().toString().split(".");
            QString menuId = slist.at(0)+"."+slist.at(1);
            ActionContainer * container = ActionManager::instance()->actionContainer(Constant::MENU_VIEW);
            if(container){
                char mid[128] = {0};
                memcpy(mid,menuId.toLocal8Bit().data(),menuId.toLocal8Bit().size());
                ActionContainer * moduleMenu = ActionManager::instance()->actionContainer(mid);
                if(moduleMenu == NULL){
                    QAction * moduleAction = new QAction();
                    moduleAction->setText(m_pluginGroupName.value(QString(mid)));
                    Action * module = ActionManager::instance()->registAction(mid,moduleAction);
                    container->appendGroup(mid);
                    container->addAction(module,mid);

                    moduleMenu = ActionManager::instance()->createMenu(mid);
                    moduleMenu->appendGroup(mid);

                    moduleAction->setMenu(moduleMenu->menu());
                }
                Action * toggleAction = ActionManager::instance()->registAction(comp->id(),dockContainer->toggleViewAction());
                moduleMenu->addAction(toggleAction,mid);
            }
        }
#endif

        iter++;
    }

    //更新插件间订阅信息
    GlobalMailBox::instance()->updateTopicMap(topicPluginMap);

    //更新系统及插件间订阅信息
    GlobalMailBox::instance()->updateSystemTopicMap(msgSystemMap);
}

/*!
 * @brief 初始工具插件
 */
void MainWindow::initToolPlugin()
{
    PluginManager::ToolComponentMap t_toolsMap = PluginManager::instance()->getAllAvailbleToolPlugins();

    PluginManager::ToolComponentMap::iterator t_toolIter = t_toolsMap.begin();
    while(t_toolIter != t_toolsMap.end()){
        RToolComponent * comp = t_toolIter.value();
        if(comp->initialize(this)){

        }else{
            RLOG_ERROR("Initialize tools %s error!",comp->id().toString().toLocal8Bit().data());
        }
        t_toolIter++;
    }
}

/*!
 * @brief 加载默认窗口布局
 * @details 此操作在程序初始化结束后进行
 */
void MainWindow::displayResize()
{
#ifdef OFFICAL_DOCK
    OfficalLayoutManager::instance()->resetDefaultLayout(RGlobal::G_GlobalConfigFile->systemConfigInfo.layout);
    showMaximized();
#else
    layoutManager->importView();
    RGlobal::G_GlobalConfigFile->systemConfigInfo.layout = layoutManager->getCurrentLayout();
    RGlobal::G_GlobalConfigFile->saveFile();
#endif

    if(!RGlobal::G_GlobalConfigFile->systemConfigInfo.fullscreen)
        showMaximized();

	raise();
}

#ifdef OFFICAL_DOCK
/*!
 * @brief 按照分组、类型添加自定义的dock窗口;
 * @attention 【需由各个插件手动调用此窗口，以注册位置】
 * @details 系统可根据groupId和typeId进行统一更新布局；
 *          1.如按站布局，则根据groupId把同一组内的窗口并列显示；
 *          2.若按数据布局，则将统一typeId的放在一个tab窗口内；
 *          3.若按默认态势布局，则显示地图和解译结果信息
 * @param[in] groupId 窗口所属分组ID
 * @param[in] typeId 窗口类型ID
 * @param[in] initialArea 初始窗口位置
 * @param[in] dockwidget 窗口所属分组ID
 */
void MainWindow::addDockWidget(QString groupId, QString typeId, Qt::DockWidgetArea initialArea, QDockWidget *dockwidget)
{
    if(dockwidget == nullptr || groupId.isEmpty() || typeId.isEmpty())
        return;

    OfficalDockItem item;
    item.m_groupId = groupId;
    item.m_typeId = typeId;
    item.m_initialArea = initialArea;
    item.m_dockWidget = dockwidget;

    OfficalLayoutManager::instance()->addDockItem(item);
}

/*!
 * @brief 移除指定的dock窗口
 * @details 若插件初始化失败时，需手动移除dock窗口
 * @param[in] groupId 窗口所属分组ID
 * @param[in] typeId 窗口类型ID
 */
void MainWindow::removeDockWidget(QString groupId, QString typeId)
{
    if(groupId.isEmpty() || typeId.isEmpty())
        return;

    OfficalLayoutManager::instance()->removeDockItem(groupId,typeId);
}
#endif

/*!
 * @brief 分发TCP解析后的数据至对应订阅模块
 * @details 1.尽可能多的将多个数据从待处理缓冲区内取出;
 *          1.1.对各个协议进行统计
 *          2.封装消息为网络消息;
 *          3.将消息投递至订阅插件
 * @attention 因子线程无法直接操作界面ui，需要由主线程代为通知
 */
void MainWindow::dispatchProtocolData()
{
    //[1]
    std::list<Core::ParserResult *> buff_Msg = RGlobal::G_ParsedProtocols.takeAll();

    std::list<Core::ParserResult *>::iterator iter = buff_Msg.begin();
    while(iter != buff_Msg.end()){

        ParserResult * result = *iter;

        ProtocolParseThread::MProtocolList * list = RSingleton<ProtocolParseThread>::instance()->getProtocolObservers(result->getNetworkId(),result->getProtocolType());

        if(list == nullptr){
            delete result;
        }else{
            QStringList targetLists;
            std::for_each(list->begin(),list->end(),[&](ProtocolParseThread::ModuleProtocol module){
                targetLists.append(module.moduleId);
            });

            //[1.1]
            if(RGlobal::G_NetCardStaticMap.contains(result->getNetworkId())){
                NetCardStaticInfo &tinfo = RGlobal::G_NetCardStaticMap.operator [](result->getNetworkId());
                if(tinfo.m_protocolStaticsInfo.contains(result->getProtocolType())){
                    ProtocolDataStaticInfo & tprotoInfo = tinfo.m_protocolStaticsInfo.operator [](result->getProtocolType());
                    tprotoInfo.m_ulRecvPackets++;
                }
            }

            //[2]
            Datastruct::PluginMessage * message = new Datastruct::PluginMessage;
            message->type = Datastruct::M_Network;
            message->targetId = targetLists.join(";");
            message->refCount = targetLists.size();         //20190326将计数改为QAtomicInt
            message->message.topicId = QString::number(result->getProtocolType());
            message->message.data = static_cast<void *>(result);

            //[3]
            for(QString tmpTargetId:targetLists){
                RComponent * taregetPlugin = PluginManager::instance()->getActivePlugin(tmpTargetId);
                if(taregetPlugin)
                    taregetPlugin->recvMail(message);
            }
        }
        iter++;
    }

    return;

    qDebug()<<"******************************************************";
    QMapIterator<QString,NetCardStaticInfo> titer(RGlobal::G_NetCardStaticMap);
    while(titer.hasNext()){
        titer.next();
        qDebug()<<"==="<<titer.key();
        std::for_each(titer.value().m_protocolStaticsInfo.begin(),titer.value().m_protocolStaticsInfo.end(),[&](ProtocolDataStaticInfo info){
            qDebug()<<info.m_name<<info.m_iType<<info.m_ulRecvPackets;
        });
    }
}

/*!
 * @brief 分发UDP、TCP原始数据至对应订阅模块
 * @attention 因子线程无法直接操作界面ui，需要由主线程代为通知。
 */
void MainWindow::dispatchProtocolData(Datastruct::OriginalArray data)
{
    RComponent * comp = PluginManager::instance()->getActivePlugin(data.mid);
    if(comp)
        comp->onNetWork(data.data,data.ip);
}

/*!
* @brief 注册插件协议至网络线程和数据解析线程
* @attention 只在创建所有网络线程后，调用
* @details 因20190911支持动态开启网络，数据注册需单独调用
*/
void MainWindow::registPluginToNetworkAndParseThread()
{

}

/*!
 * @brief 控制网卡运行状态
 * @param[in] networkid 网卡名
 * @param[in] running 待设置的运行状态
 */
void MainWindow::conrtorlNetworkState(QString networkId, bool running)
{

}

/*!
* @brief 网络状态改变
* @param[in] currState 最新网络状态
*/
void MainWindow::respNetStateChanged(NetworkState currState)
{
    Datastruct::NetworkState * state = RGlobal::G_NetworkState.value(currState.netId);
    if(state == NULL){
        Datastruct::NetworkState * networkState = new Datastruct::NetworkState;
        *networkState = currState;
        G_NetworkState.insert(currState.netId,networkState);
    }else{
        *state = currState;
    }

    if(currState.isError){
        RMessageBox::warning(this,QStringLiteral("警告"),currState.error,RMessageBox::Yes);
    }

    emit netStateChanged();
}

/*!
 * @brief 创建新网卡
 * @details 1.若创建成功，直接启动网络任务；
 *          2.若创建失败，不启动，但将任务加至任务线程
 * @param[in]   netInfo 网卡信息
 */
void MainWindow::createNetwork(Core::Datastruct::NetworkInfo ninfo)
{

}

/*!
 * @brief 删除指定网卡信息
 * @details 因大部分情况下仅依据name来判断网卡是否唯一，但为了避免将删除的网卡，再次创建但其配置信息不一致的问题;
 *          在删除时候采取更加详细的检测依据
 * @param[in] ninfos 待删除网卡集合
 */
void MainWindow::delteNetwork(QList<Core::Datastruct::NetworkInfo> ninfos)
{
    std::for_each(ninfos.begin(),ninfos.end(),[&](Datastruct::NetworkInfo info){
        //TODO 20190123
    });
}

/*!
 * @brief 处理由插件发送至框架的信息
 * @param[in]  message 插件消息
 * @author wey
 * @date 2019-03-26
 */
void MainWindow::processToSystemMessage(PluginMessage *message)
{
    if(message){
        if(message->message.topicId == QString(SystemMsg::plugin_topic_parsedlimit)){
            ParseLimitList * t_state = static_cast<ParseLimitList *>(message->message.data);
            RSingleton<ProtocolParseThread>::instance()->modifyParseLimit(t_state);
            G_SafeReleaseMessage<ParseLimitList>(message);
        }else if(message->message.topicId == QString(SystemMsg::plugin_topic_networkParamterUpdate)){
            DeviceNetworkUpdateList * t_networkList = static_cast<DeviceNetworkUpdateList *>(message->message.data);
            ProtocolManager::instance()->updateProtocolIp(t_networkList);
            G_SafeReleaseMessage<DeviceNetworkUpdateList>(message);
        }
    }
}

}//Core
