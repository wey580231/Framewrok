/*!
 *  @brief     协议管理器
 *  @details   1.负责维护所有解析产生的协议数据，每个协议之间不存在相互的影响 @n
 *             2.通过向网络数据处理以及显示模块提供解析后的字段内容 @n
 *  @author    wey
 *  @version   1.0
 *  @date      2018.11.07
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note 20190409:wey:增加对多个同一个协议类型支持
 */
#ifndef PROTOCOLMANAGER_H
#define PROTOCOLMANAGER_H

#include <QMap>
#include <core/core_global.h>
#include "datastruct.h"
#include "protocolField.h"
#include "waitimplemneted.h"

namespace Core{

class CORESHARED_EXPORT ProtocolManager
{
public:
    static ProtocolManager* instance();

    /*!< 解析xml协议 */
    bool parseXmlInLocalDir(QString dirname,ProtocolParser &parser);
    bool parseLocalProtocol(QString fullFilePath,ProtocolParser &parser);

    /*!< 判定协议是否存在 */
    int protocolCount(const QString &networId, int protocolType);
    bool isExisted(const QString &networId,int protocolType);

    /*!< 更新xml协议 */
    void updateProtocolIp(DeviceNetworkUpdateList * list);

    const Core::BaseProtocol* getProtocol(QString protocolName,bool *isExisted);
    const Core::BaseProtocol* getProtocol(const QString &networId,int protocolType,bool *isExisted);
    const Core::BaseProtocol* getProtocol(const QString &networId,int protocolType, QString ip, bool * existed);

    int size(){return protocols.size();}

    typedef QMap<QString,Core::BaseProtocol*> BaseProtocolMap;          /*!< key:xml协议中name属性，唯一的协议名，value：对应的解析协议 */
    const BaseProtocolMap getProtocolMap(){return protocols;}

private:
    QStringList getXmlsFullPath(QString networkId);

private:
    ProtocolManager();
    static ProtocolManager *m_instance;

    BaseProtocolMap protocols;
};

}

#endif // PROTOCOLMANAGER_H
