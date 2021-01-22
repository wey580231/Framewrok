/*!
 *  @brief     类型定义
 *  @details   定义了一些配置的基本字段
 *                  【1】：数据库，系统配置
 *                  【2】：界面布局信息
 *                  【3】：可配置窗口支持字段
 *                  【4】：插件支持
 *  @file      datastruct.h
 *  @author    wey
 *  @version   1.0
 *  @date      2018.03.02
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note 20190110:wey:调整插件网络模块，增加区分接收和发送
 */
#ifndef DATASTRUCT_H
#define DATASTRUCT_H

#include <functional>
#include <iomanip>

#include <QDebug>
#include <QString>
#include <QWidget>
#include <QList>
#include <QVariant>
#include <QDataStream>
#include <QQueue>
#include <QRect>
#include <QDate>
#include <QTime>
class QDockWidget;

#include <core/core_global.h>
#include <commondefines/datastruct.h>

#include "protocolField.h"

using namespace std;

#define RECV_BUFF 1024*64       /*!< 网络接收缓冲区空间，Tcp、Udp使用 */
#pragma pack(push)
#pragma pack(1)

/*********************************************DOCK窗口********************************************************/
#define MOUSE_TRIGGER_SPACE 5        /*!< 窗口各边对鼠标事件触发间距，插件最外层边距 */

namespace Core{

namespace DataStrct_Prototype{
struct Protocol;
}


namespace Datastruct {

struct Field;
struct PubHead;

/*!
 *  @brief 系统支持的服务类型
 *  @details 用户以命令行方式输入对应的服务.
 */
enum ServiceType
{
    SERVICE_TEXT,               /*!< 使用传输文本服务,默认类型 */
    SERVICE_FILE                /*!< 使用传输文件服务 */
};

/*!
 *  @brief 数据库类型
 */
enum DatabaseType
{
    DB_NONE = 0,
    DB_MYSQL,               /*!< 使用MySql数据库*/
    DB_ORACLE               /*!< 使用Oracle数据库*/
};

/*!
 * @brief 解析结果
 */
enum ResultType
{
    PARSE_ERROR,            /*!< 发生错误*/
    VIEW_PROGRAM,           /*!< 查看程序信息*/
    EXEC_PROGRAM            /*!< 执行程序*/
};

struct CommandParameter
{
    CommandParameter()
    {
        serviceType = SERVICE_TEXT;
        dbType = DB_MYSQL;
        parseResult = PARSE_ERROR;
    }

    ServiceType serviceType;
    DatabaseType dbType;
    ResultType parseResult;
};


/*!
 *  @brief 数据库特征信息
 *  @note 保存当前使用的数据库中的功能特点
 */
struct DBFeature
{
    bool lastInsertId;          /*!< 是否支持查询最后插入数据的id */
    bool transactions;          /*!< 是否支持事务 */
};

/*!
 *  @brief 数据库配置信息
 */
struct DatabaseConfigInfo
{
    DatabaseType dbType;
    QString hostName;
    QString dbName;
    QString dbUser;
    QString dbPass;
    ushort port;
};

/*!
 *  @brief 系统配置信息
 */
struct SystemConfigInfo
{
    SystemConfigInfo():defaultKeySchemes(true),fullscreen(true),topHint(false),menubarVisible(false),m_bStatusBarVisible(false),m_bNetworkAutoCreateInStartProcess(false)
    ,m_bDebugNetParsedInfo(false){
    }
    bool defaultKeySchemes;             /*!< 是否采用默认的快捷键设置，默认为true */
    bool fullscreen;                    /*!< 是否采用全屏显示 */
    bool topHint;                       /*!< 是否置顶显示 */
    bool menubarVisible;                /*!< 菜单栏是否显示 */
    bool m_bStatusBarVisible;           /*!< 状态栏是否显示 20190814 */
    bool m_bNetworkAutoCreateInStartProcess;        /*!< 网络是否随程序启动时自动启动 20190911 */
    bool m_bDebugNetParsedInfo;         /*!< 调试网络解析 */
    bool m_bAppAutoStart;               /*!< 程序是否开机启动 20191213 */
    QString userKeySchemesName;         /*!< 自定义快捷键设置名称 @see defaultKeySchemes 为false时有效 */
    QString locale;                     /*!< 显示的语言 */
    QString style;                      /*!< 默认样式 */
    QString layout;                     /*!< 上一次布局 */
};

/*!
 *  @brief 系统工作模式
 *  @details 默认为WORK_RealData模式，处理实时网络数据，数据记录
 *           在数据回访时切换至WORK_PlayBack模式，此时网络数据不处理，回放产生的数据不记录。
 */
struct SystemWorkState{
    SystemWorkState(){
        mWorkMode = WORK_RealData;
        m_b_Forward = false;
    }

    enum WorkState{
	    WORK_RealData,         /*!< 实时工作模式 */
	    WORK_PlayBack          /*!< 回放模式 */
    }mWorkMode;
	
    bool m_b_Forward;          /*!< 数据转发 */
};

/*!
 *  @brief 海图配置信息
 */
struct MapConfigInfo
{
    MapConfigInfo():refreshInterval(1000){}
    QString useHtmlMap;
    QString mapPath;
    int refreshInterval;
};

/*********************************************系统消息********************************************************/
enum MessType{
    M_System,          /*!< 系统消息 */
    M_Plugin,          /*!< 插件间消息 */
    M_Network          /*!< 网络系统消息 */
};

/*!
 *  @brief 插件通信结构信息
 *  @details 此结构用于插件-插件、系统-插件之间的信息传输单元; @n
 *      发送者向接收者发送信息前，通过接收端id填入targetId;
 *      根据消息的类型，填写topicId和对应的message:
 *          [1]当type为M_System时，填写topicId，根据传播范围填写m_broadCast字段信息; @n
 *          [2]当type为M_Plugin时，可填写topicId或data字段，接收者根据约定去二者之一取值; @n
 *          [3]当type为M_Network时，topicId字段为网络协议类型，data为数据字段; @n
 *      发送者将信息信息填写好后，将信息投递给全局的邮箱，全局的邮箱根据消息目的Id，找到对应的插件或插件集合，将信息投递至目的地的邮箱 @n
 *  @note 20190118:为解决主线程轮寻时需等待插件处理业务数据，造成页面卡顿严重问题.
 *        20190326:将引用计数器从int类型修改为QAtomicInt类型
 */
struct PluginMessage{
    PluginMessage(){
        message.data = nullptr;
        message.m_broadCast = false;
    }
    ~PluginMessage(){}

    QString  targetId;          /*!< 消息目的ID,多个目的地址的id使用分号;连接 */
    QAtomicInt refCount;        /*!< 消息引用的数量，当到达0时，可删除。 */
    MessType type;              /*!< 消息类型 */
    struct{
        bool m_broadCast;       /*!< 消息是否为广播,当type为M_System有效 */
        QString topicId;        /*!< 主题类型，可参见related/topliclist.h或core/topliclist.h(插件发送至框架)文件 */
        void * data;            /*!< 消息内容区，目的插件可根据消息类型，手动转换消息至对应的类型 */
        int dataSize;           /*!< 消息内容长度 */
    }message;
};

typedef QList<Datastruct::PluginMessage *> PluginMessageList;

/*!
 *  @brief 对消息引用数量减1，若引用数量<=0,则需删除消息。删除消息时，同步删除T类型的消息数据区
 *  @details 20190326:消息内容通过void*保存，不能直接使用delete删除，需要将void*转换成对应的数据类型后再删除，
 *                    这样可以确保对应的析构函数可以执行;
 */
template<class T>
void G_SafeReleaseMessage(Datastruct::PluginMessage * &pmessage){
    if(pmessage != nullptr){
        if(pmessage->refCount.load() > 0)
            pmessage->refCount.deref();

        if(pmessage->refCount.load() <= 0){
            if(pmessage->message.data != nullptr){
                T * t_tmp = static_cast<T *>(pmessage->message.data);
                delete t_tmp;
            }
            delete pmessage;
            pmessage = nullptr;
        }
    }
}

/*!
 *  @brief 系统状态信息等级
 */
enum StatusLevel{
   Error,
   Warning
};

/*!
 *  @brief 系统运行时产生的状态信息
 */
struct SystemStatusInfo{
    StatusLevel m_level;
    QString m_content;          /*!< 消息内容 */
    QString m_src;              /*!< 信息来源(框架、插件)，可针对此过滤 */
    QString m_timestamp;        /*!< 时间戳:只存在时分秒 */
};

/************************************************布局信息*****************************************************/
/*!
 *  @brief dock窗口拖动嵌入时状态
 */
enum DockDragState{
    NoDrag,            /*!< 初始状态 */
    DragStart,         /*!< 开始拖动 */
    DragMove,          /*!< 拖动后移动 */
    Drop,              /*!< 放下 */
    DragEnter,         /*!< 拖动进入 */
    DragLeave,         /*!< 拖动离开 */
    DragTerminate      /*!< 终止拖动 */
};

enum ItemType{
    DockType,
    TabType
};

struct BaseLayoutItem{
    BaseLayoutItem(){}
    BaseLayoutItem(const BaseLayoutItem & item){
        this->itemType = item.itemType;
        this->objName = item.objName;
        this->geometry = item.geometry;
        this->feature = item.feature;
    }
    ItemType itemType;
    QString objName;
    QRect geometry;         /*!< 窗口实际尺寸,未设置expand属性时后的尺寸*/
    int feature;

    friend QDataStream & operator << (QDataStream & stream,BaseLayoutItem & item);
    friend QDataStream & operator >> (QDataStream & stream,BaseLayoutItem & item);
};

/*!
 *  @brief 嵌入TabWidget的基础信息
 */
struct TabInfo{
    TabInfo():tabbed(false){}

    bool tabbed;            /*!< 是否和其它item嵌入,true时表示需要创建TabWidget用以容纳自身和其它对象 */
    QString tabObjName;     /*!< 组合后的tabWidget的objName */
    friend QDataStream & operator << (QDataStream & stream,TabInfo & item);
    friend QDataStream & operator >> (QDataStream & stream,TabInfo & item);
};

/*!
 *  @brief dock与普通widget的描述信息
 */
class DockItem : public BaseLayoutItem{
public:
    DockItem():BaseLayoutItem(),expanded(false){
        this->itemType = DockType;
    }
    DockItem(const DockItem & item):BaseLayoutItem(item){
        this->itemType = DockType;
        this->expanded = item.expanded;
        this->tabInfo = item.tabInfo;
    }
    bool expanded;          /*!< 是否停靠在边,dockwidget时有效 */
    TabInfo tabInfo;
    friend QDataStream & operator << (QDataStream & stream,DockItem & item);
    friend QDataStream & operator >> (QDataStream & stream,DockItem & item);
};

/*!
 *  @brief MyTabWidget保存的布局信息
 */
class TabItem : public BaseLayoutItem{
public:
    TabItem():BaseLayoutItem(),tabPosition(0){
        this->itemType = TabType;
    }
    TabItem(const TabItem & item):BaseLayoutItem(item){
        this->itemType = TabType;
        this->tabPosition = item.tabPosition;
        this->currentIndex = item.currentIndex;
        this->tabPageObjNames = item.tabPageObjNames;
    }
    int tabPosition;        /*!< Tab标签页方向，值参见QTabWidget::TabPosition */
    int currentIndex;       /*!< 置顶显示的索引 */
    QStringList tabPageObjNames;    /*!< 子页面的objName集合 */

    friend QDataStream & operator << (QDataStream & stream,TabItem & item);
    friend QDataStream & operator >> (QDataStream & stream,TabItem & item);
};


typedef QQueue<ProtocolArray> ProtocolQueue;
typedef std::function<void(Datastruct::ProtocolArray &)> DataHandler;

typedef QQueue<OriginalArray> OriginalQueue;

/*!
 * @brief Udp数据发送单元
 * @details UDP数据发送由专门的UDP任务线程统一发送
 */
struct UdpSendUnit{
    QString networkId;          /*!< 网络模块id */
    QByteArray data;            /*!< 待发送数据体 */
};

typedef QQueue<UdpSendUnit> UdpSendQueue;

/*!
 *  @brief 官方单个dock配置信息
 *  @attention 此属性只在定义了OFFICAL_DOCK宏下有效
 *  @details 因TT后端需要对三站数据窗口进行统一布局，自定义的MyDockWidget不太灵活，遂使用官方进行布局管理
 *  @date 2019-11-04
 */
struct OfficalDockItem{
    OfficalDockItem():m_dockWidget(nullptr){}
    QString m_groupId;          /*!< dock所属分组id；相同分组id的窗口会被以tab进行布局 */
    QString m_typeId;           /*!< dock类型id；相同typeId的可以标识窗口的功能一致 */
    Qt::DockWidgetArea m_initialArea;   /*!< 初始dock窗口位置 */
    QDockWidget * m_dockWidget; /*!< dock窗口 */
};

/********************************************窗口属性设置*******************************************************/

/*!
 *  @brief 布局类型
 *  @details 20181030暂支持3种类型
 */
enum Layout
{
    None,
    Horizonal,          /*!< Gridlayout布局一行，多列 */
    Vertical,           /*!< Gridlayout布局一列，多行 */
    Grid,               /*!< Gridlayout布局多行，多列 */
    RTab                /*!< 布局多页 */
};

/*!
 *  @brief 窗口数据信息
 */
struct WindowData{
    WindowData(){
        isShown = true;
        isEnabled = true;
        column = 1;
    }

    QString  name;        /*!< 控件前面label显示信息 */
    ControlType type;     /*!< 界面类型 */
    Layout layout;        /*!< 布局类型 */
    bool isShown;         /*!< 可进行显示 */
    bool isEnabled;       /*!< 可编辑 */
    ushort column;        /*!< 网格布局时显示的列数 */
    ushort width;         /*!< 宽度 */
    ushort height;        /*!< 高度 */
};

/*!
 *  @brief 容器信息显示描述
 */
struct Window : public PubHead{
    Window():PubHead(){ }
    ~Window(){}
    WindowData data;       /*!< 窗口属性 */
};

/*!
 *  @brief 节点容器
 *  @details 1.容器可嵌套；
 *           2.大的容器中可包含子容器，由 @see dataElem 来定义；
 *           3.最内的容器只包含字段信息
 */
struct Container{
    Container():autoLayout(true),taskId(0){}
    Window     continer;                  /*!< 描述当前容器的属性 */
    int taskId;                           /*!< 协议序号，只在最外层容器中值有效 */
    bool autoLayout;                      /*!< 是否自动布局,默认true自动布局。当前为false时，根据解析的对应的name在已注册的类中查找窗体 */
    QString sendNetId;                    /*!< 控制指令目的网卡 */
    QList<Field*> fileds;                 /*!< 节点集合 */
    QList<Container*>  childContainer;    /*!< 子容器 */
};

/****************************************插件控制信息**************************************************/

/*!
 *  @brief 插件描述信息
 *  @details
 */
struct PluginInfo{
    QString id;         /*!< 插件标识 */
    QString name;       /*!< 名称 */
};

enum NetworkType{
    N_TCP,
    N_UDP
};

/*!
 *  @brief 网络连接类型
 */
enum NetworkConnectionType{
    N_Server,
    N_Client
};

/*!
 *  @brief 网络基本信息
 */
struct NetworkBase{
    NetworkBase():multicast(false),recordNetFile(false),singleFileSize(128){}

    bool operator== (const NetworkBase & baseInfo){
        if(this == &baseInfo)
            return true;
        return this->ip == baseInfo.ip && this->port == baseInfo.port && this->connectionType == baseInfo.connectionType;
    }

    QString ip;
    ushort port;
    NetworkConnectionType connectionType;
    bool multicast;         /*!< 是否开启组播 udp有效*/
    QString multicastIp;    /*!< 组播Ip */
    bool recordNetFile;     /*!< 记录网络数据，tcp有效 */
    QString recordNetPath;  /*!< 保存网络数据文件路径 */
    int singleFileSize;     /*!< 单个文件大小(M) */
};

/*!
 *  @brief 网络配置信息
 *  @details 初始化网络模块
 */
struct NetworkInfo{

    bool operator== (const NetworkInfo & info){
        if(this == &info)
            return true;

        return this->id == info.id && this->protocol == info.protocol && this->baseInfo== info.baseInfo;
    }

    QString id;             /*!< 网络Id */
    NetworkType protocol;   /*!< 传输类型 */
    NetworkBase baseInfo;   /*!< 基础信息 */
};

/*!
 *  @brief 数据转发结构
 *  @details 描述转发映射关系，将本地指定ip和端口的数据，转发至远程指定的ip和端口
 *           分别指定本地数据接收以及远程数据接收的参数，可实现tcp-tcp、tcp-udp、udp-udp、udp-tcp四种转换
 *           txt中格式为[源ip]:[源port]:[目的ip]:[目的port]
 *           1.源ip可设置为某一具体ip，也可设置为any，表示任一ip；
 *           2.源port可设置为某一具体port(只有通过此端口来的数据被转发)，也可设置为any，表示任一port；
 *  @author wey
 *  @date 2019-04-17
 */
struct CORESHARED_EXPORT ForwardConfigInfo{
    QString id;             /*!< 网络ID */

    NetworkType m_localProtocol;   /*!< 本地接收数据协议类型 */
    QString m_localIp;      /*!< 本地ip */
    ushort m_localPort;     /*!< 本地port */

    NetworkType m_remoteProtocol; /*!< 远程接收数据协议类型 */
    QString m_remoteIp;     /*!< 远程数据Ip */
    ushort m_remotePort;   /*!< 远程数据端口 */
};
typedef CORESHARED_EXPORT QList<Datastruct::ForwardConfigInfo> ForwardList;

/*!
 *  @brief 网卡运行状态信息
 */
struct NetworkState{
    NetworkState():running(false),isError(false){

    }

    QString netId;          /*!< 网络Id */
    bool running;           /*!< 是否处于运行状态 */
    bool isError;           /*!< 是否运行错误 */
    QString error;          /*!< 网卡初始化错误信息 */
};

enum WindowLayout{
    TOP,
    BOTTOM,
    LEFT,
    RIGHT
};

/*!
 *  @brief 网络配置信息
 *  @details 插件可自己配置多个接收、发送的网路链路
 *  @details 20190410:wey:调整多网卡配置，将网卡和具体的协议绑定(适应铁塔项目)
 */
struct NetworkConfig{
    QString recvNetworkId;  /*!< 接收数据网卡ID */
    QString sendsNetworkId; /*!< 发送数据网卡ID */
    QStringList protocols;  /*!< 协议集合， @see  ProtocolInfo */
};

typedef QList<NetworkConfig> NetworkConfigList;

/*!
 *  @brief  模块信息
 *  @details 对应plugin.xml中单个<module/>节点
 */
struct ModuleInfo{
    WindowLayout layout;    /*!< 在mainwindow中的位置方向 */
    QRect geometry;         /*!< 尺寸信息 */
    bool closeable;         /*!< 是否可关闭 */
    bool visible;           /*!< 是否可见 */
    bool floatable;         /*!< 是否可移动 */
    QString name;           /*!< 模块名称 */
    QString pluginId;       /*!< 内部插件ID @see PluginInfo */
    NetworkConfigList netconfigs;  /*!< 网络模块集合， @see BaseProtocol */
};

typedef QList<Datastruct::ModuleInfo> ModuleList;
typedef QMap<QString,Datastruct::NetworkInfo> NetworkMap;       /*!< key:网卡id，value：网卡配置信息 */
typedef QMap<QString,Datastruct::PluginInfo> PluginMap;


/*!
 * @brief Tcp/Udp注册待需处理数据结构信息
 */
struct ModuleProtocol{
    int startCode;      /*!< 开始标志码 */
    int type;           /*!< 协议类型 */
    int length;         /*!< 整包数据长度 */
    int endCode;        /*!< 结束标志码 */
    QString moduleId;   /*!< 模块Id */
};

/*!
 *  @brief 数据上报网络信息
 */
struct ReportNetSetting{
    ReportNetSetting():remotePort(0),autoReport(false){}

    QString localIp;
    QString remoteIp;
    ushort remotePort;
    bool autoReport;        /*!< 是否手动上报 */
    bool recordFile;        /*!< 是否要记录文件 */
    QString recordFilePath;      /*!< 中频DDR文件保存路径 */
};

/*!
 *  @brief 串口基本配置
 */
struct SerialPortSetting{
    SerialPortSetting():baudRate(9600),dataBits(8),parity(2),stopBits(1),recordFile(false),singleFileSize(128){}
    QString serialport;     /*!< 串口号 */
    int baudRate;           /*!< 波特率 */
    int dataBits;           /*!< 数据位 */
    int parity;             /*!< 校验位 */
    int stopBits;           /*!< 停止位 */
    bool recordFile;        /*!< 记录网络数据 */
    QString recordPath;     /*!< 保存网络数据文件路径 */
    int singleFileSize;     /*!< 单个文件大小(M) */
};

/*!
 *  @brief 协议数据统计信息
 *  @date 2019-09-11
 */
struct ProtocolDataStaticInfo{
    ProtocolDataStaticInfo(){}
    ProtocolDataStaticInfo(QString netId,QString recvIp,QString name,int type):m_netId(netId),m_ip(recvIp),m_name(name),m_iType(type),m_ulRecvPackets(0){}
    QString m_netId;                /*!< 接收网卡ID */
    QString m_ip;                   /*!< 绑定的ip */
    QString m_name;                 /*!< 协议名 */
    int m_iType;				/*!< 协议类型 */
    qint64 m_ulRecvPackets;     /*!< 当前协议下数据包个数 */
};

typedef QList<ProtocolDataStaticInfo> ProtocolDataStaticInfoList;
typedef QMap<int,ProtocolDataStaticInfo> ProtocolDataStaticInfoMap;

/*!
 *  @brief 单个网卡统计信息
 */
struct NetCardStaticInfo{
    QString m_networkId;    /*!< 唯一标识 */
    QString m_ip;           /*!< ip */
    int m_iPort;            /*!< 端口 */
    ProtocolDataStaticInfoMap m_protocolStaticsInfo;        /*!< 单个网卡上协议统计信息 */
};

typedef QMap<QString,NetCardStaticInfo> NetCardStaticMap;       /*!< key:网卡id */


/****************************************网络状态检测**************************************************/
/*!
 *  @brief 客户端网络信息
 */
struct ClientBaseInfo{
    ClientBaseInfo():sockId(-1),port(0),recvBytes(0),sendBytes(0),sendCacheSize(0),recvCacheSize(0){}
    int sockId;             /*!< sock句柄 */
    unsigned short port;    /*!< 端口号 */
    quint64 recvBytes;      /*!< 接收字节数 */
    quint64 sendBytes;      /*!< 发送字节数 */
    int sendCacheSize;      /*!< 发送缓冲区大小 */
    int recvCacheSize;      /*!< 接收缓冲区大小 */

    QString networkId;      /*!< 网卡ID -默认无*/
    QString ip;             /*!< ip - 默认任意 */
};

/*!
 *  @brief 服务器端网络基本信息
 */
struct ServerBaseInfo{
    QString networkId;      /*!< 网卡ID */
    QString ip;             /*!< ip */
    unsigned short port;    /*!< 监听端口号 */
    QList<ClientBaseInfo> clients;      /*!< 客户端信息 */
};

/****************************************协议解析**************************************************/
/*!
 *  @brief 协议条目解析限制
 *  @details 为了应对全脉冲等同一时刻传输几十万条数据，对解析的条目进行限制。避免一次性全部解析，提高解析显示效率
 *  @date 2019-07-26
 */
struct ProtocolParseLimit{
    ProtocolParseLimit():m_bValid(false),protocolType(-1){}
    bool m_bValid;              /*!< 此解析条件是否有效 */
    int protocolType;           /*!< 数据协议类型 */
    QList<int> m_protoLimits;   /*!< 对应协议解析条数限制，-1表示无限制，> 0表示只解析对应数量的数据，超出的数据将被跳过；按照xml中body节点包含items的顺序设置。 */
};

typedef QList<ProtocolParseLimit> ParseLimitList;

/*!
 *  @brief 单个设备网络参数信息更新
 *  @details 为了支持下发网络参数更新指令，需同步更新本地的plugins.xml和对应设备上传输的协议xml文件。
 *  @attention 本操作需与xml中的网络节点的名称、本地协议的文件名(如敌我全脉冲1030.xml)耦合
 *  @date 2019-10-25
 */
struct DeviceNetworkUpdateInfo
{
    NetworkType m_networkType;      /*!< 1.为tcp时，需要将上位机ip和port更新至plugin.xml、将设备的ip更新至协议xml中ip属性
                                         2.为udp时，需要将设备的ip和port更新至plugins.xml中   */
    QString m_systemNetworkId;      /*!< 上位机plugins.xml中网卡id */

    QString m_systemListenIp;       /*!< 上位机监听的ip和port,若为空则不更新对应plugin.xml文件 */
    ushort m_systemListenPort;

    QString m_deviceListenIp;       /*!< 设备监听的ip和port */
    ushort m_deviceListenPort;

    QMap<QString,QString> m_protocolAttributesMap;     /*!< xml协议中需更新的属性集合；key:协议属性名，value:协议属性值。tcp时有效 */
};

typedef QList<DeviceNetworkUpdateInfo> DeviceNetworkUpdateList;

/****************************************地图标绘**************************************************/
/*!
 * @brief 地图操作状态
 * @details 每种操作状态均可通过工具栏设置，不同的状态可能存在互斥
 */
enum MapOperateType{
    Map_None = 0x0000,
    Map_Move = 0x0001,              /*!< 可拖拽 */
    Map_Zoom = 0x0002,              /*!< 可缩放 */
    Map_MouseTracking = 0x0004,     /*!< 鼠标跟踪 */
    Map_Distance = 0x0008,          /*!< 测距 */
    Map_Mark = 0x0010,              /*!< 标绘 */
    Map_Clear = 0x0020,             /*!< 清空 */

    Map_First = Map_Move,
    Map_Last = Map_Mark
};

/*!
 * @brief 地图操作状态
 */
struct MapState{
    MapOperateType m_operateType;   /*!< 当前操作类型 */
    int m_mutexType;                /*!< 和当前操作互斥的类型集合 */
    QString m_imagePath;            /*!< 图标路径 */
    QString m_text;                 /*!< 提示文本 */
    bool m_b_checkeable;            /*!< 需要选中状态 */
};

#pragma pack(pop)

} //namespace Datastruct

} //namespace Core

#endif // DATASTRUCT_H
