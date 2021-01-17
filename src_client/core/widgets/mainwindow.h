/*!
 *  @brief     通用显示框架
 *  @details   以Dock方式运行子程序页面
 *  @author    wey
 *  @version   1.0
 *  @date      2018.08.22
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QList>
class QAction;

#include <core/pluginmanager/mailbox/observer.h>
#include <core/core_global.h>
#include <core/protocol/datastruct.h>

namespace Base {
class ActionContainer;
}

namespace Ui {
class MainWindow;
}

namespace Core{

class BaseInfoDockPanel;
class TitleBar;
class StatusBar;
class ModuleSetting;
class LayoutManager;
class RDockWidget;

class CORESHARED_EXPORT MainWindow : public QMainWindow , public Core::Observer
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    static MainWindow * instance();
    ~MainWindow();

    void onMessage(QString messageType);

    void initAll();
    void displayResize();

private:
    void initMenu();

protected:
    void closeEvent(QCloseEvent *event);
    void resizeEvent(QResizeEvent * event);

signals:
    void netStateChanged();

public slots:
    void registPluginToNetworkAndParseThread();
    void createNetwork(Core::Datastruct::NetworkInfo ninfo);
    void conrtorlNetworkState(QString networkId, bool running);

private slots:
    bool programExit();

    void windowTopHint(bool flag);
    void windowFullScreen(bool flag);

    void switchLayout();
    void switchStyle();
    void switchLanguage();

    void technicalSupport();
    void aboutProgram();
    void showPluginDesc();

    void showShortcutSettings();

    void dispatchProtocolData();
    void dispatchProtocolData(Datastruct::OriginalArray data);

    void respNetStateChanged(Datastruct::NetworkState  currState);
    void delteNetwork(QList<Core::Datastruct::NetworkInfo> ninfos);

    void processToSystemMessage(Datastruct::PluginMessage* message);

private:
    void initComponent();
    void initStatusBar();
    void initNetwork();
    void initBusinessPlugin();
    void initToolPlugin();

    void updateStyle(int index);
    void updateLanguage(QString lanFileName);
    void loadUserSetting();
    void retranslateUi();
    void respNetworkUpdate();

#ifdef OFFICAL_DOCK
    void addDockWidget(QString groupId,QString typeId,Qt::DockWidgetArea initialArea,QDockWidget * dockwidget);
    void removeDockWidget(QString groupId,QString typeId);
#endif

private:
    Ui::MainWindow *ui;
    static MainWindow * _mainWindow;

    QString curLanguageName;

	Base::ActionContainer * serverMenu;
	Base::ActionContainer * viewMenu;
	Base::ActionContainer * viewManagerMenu;
	Base::ActionContainer * settingsMenu;
	Base::ActionContainer * styleMenu;
	Base::ActionContainer * lanMenu;
	Base::ActionContainer * helpMenu;
    QAction * exitAction;
    QAction * importViewAction;
    QAction * exportViewAction;
    QAction * topHintAction;
    QAction * fullScreenAction;
    QAction * shortcutAction;
    QAction * supportAction;
    QAction * pluginSpecAction;
    QAction * aboutPorgramAction;

    TitleBar * titleBar;
    StatusBar * m_statusBar;                /*!< 状态栏 */
    ModuleSetting * settingBar;
    LayoutManager * layoutManager;

    QMap<QString,QString> m_pluginGroupName;        /*!< key:<PLUGIN>.<插件所属模块> value: */
};

} //namespace Core

#endif // MAINWINDOW_H
