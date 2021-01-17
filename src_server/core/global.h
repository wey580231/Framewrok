#ifndef CORE_GLOBAL_H_2018_12_27
#define CORE_GLOBAL_H_2018_12_27

#include <mutex>
#include <condition_variable>

#include <QString>
#include <QQueue>
#include <QMap>
#include <QSharedPointer>

#include <base/util/rblockingqueue.h>
#include <core/protocol/datastruct.h>
#include <core/core_global.h>

#include "protocol/datastruct.h"

namespace Core{

using namespace Datastruct;
class GlobalConfigFile;
class ParserResult;

namespace RGlobal
{

extern ServiceType G_SERVICE_TYPE;             /*!< 当前服务器运行的模式，根据不同的模式，网络数据解析存在不同 */
extern QString G_FILE_UPLOAD_PATH;             /*!< 文件上传保存的路径 */
extern DBFeature G_DB_FEATURE;                 /*!< 当前数据库驱动支持的功能 */
extern Datastruct::NetCardStaticMap G_NetCardStaticMap;         /*!< 网卡上数据统计信息 */

extern SystemWorkState CORESHARED_EXPORT G_WorkState;         /*!< 系统工作模式 */
extern SerialPortSetting CORESHARED_EXPORT G_SerialPortSetting;
extern GlobalConfigFile CORESHARED_EXPORT * G_GlobalConfigFile;
extern ReportNetSetting CORESHARED_EXPORT G_ReportNetSetting;
extern QMap<QString,Datastruct::NetworkState*> CORESHARED_EXPORT G_NetworkState;  /*!< 网卡运行状态信息 */

//基于协议的Tcp数据
extern Base::RBlockingQueque<Core::Datastruct::ProtocolArray> CORESHARED_EXPORT G_ProtocolQueue;

//保存由ProtocolParseThread解析的协议值
extern Base::RBlockingQueque<Core::ParserResult*> CORESHARED_EXPORT G_ParsedProtocols;

//基于Udp发送数据
extern Base::RBlockingQueque<Core::Datastruct::UdpSendUnit> CORESHARED_EXPORT G_Udp_SendQueue;

//基于TCP/UDP的原始数据信息
extern Base::RBlockingQueque<Core::Datastruct::OriginalArray> CORESHARED_EXPORT G_Original_RecvQueue;

//全局系统状态消息
extern Base::RBlockingQueque<Core::Datastruct::SystemStatusInfo> G_StatusInfoQueue;

}

} //namespace Core

#endif // GLOBAL_H
