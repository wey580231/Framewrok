#include "global.h"

namespace Core {

namespace RGlobal
{

ServiceType G_SERVICE_TYPE;             /*!< 当前服务器运行的模式，根据不同的模式，网络数据解析存在不同 */
QString G_FILE_UPLOAD_PATH;             /*!< 文件上传保存的路径 */
DBFeature G_DB_FEATURE;                 /*!< 当前数据库驱动支持的功能 */
Datastruct::NetCardStaticMap G_NetCardStaticMap;        /*!< 网卡上数据统计信息 */

SystemWorkState CORESHARED_EXPORT G_WorkState;
SerialPortSetting CORESHARED_EXPORT G_SerialPortSetting;
GlobalConfigFile CORESHARED_EXPORT  *G_GlobalConfigFile = nullptr;
ReportNetSetting CORESHARED_EXPORT G_ReportNetSetting;

QMap<QString,Datastruct::NetworkState*> CORESHARED_EXPORT  G_NetworkState;

//基于协议的Tcp数据
Base::RBlockingQueque<Core::Datastruct::ProtocolArray> CORESHARED_EXPORT G_ProtocolQueue;

//保存由ProtocolParseThread解析的协议值
Base::RBlockingQueque<Core::ParserResult*> CORESHARED_EXPORT G_ParsedProtocols;

//基于Udp发送数据
Base::RBlockingQueque<Core::Datastruct::UdpSendUnit> CORESHARED_EXPORT G_Udp_SendQueue;

//基于TCP/UDP的原始数据信息
Base::RBlockingQueque<Core::Datastruct::OriginalArray> CORESHARED_EXPORT G_Original_RecvQueue;

//全局系统状态消息
Base::RBlockingQueque<Core::Datastruct::SystemStatusInfo> G_StatusInfoQueue;

} //namespace RGlobal

} //namespace Core

