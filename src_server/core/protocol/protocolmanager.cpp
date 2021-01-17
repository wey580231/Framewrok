#include "protocolmanager.h"

#include <QDir>
#include <QDebug>
#include "../pluginmanager/plugin/pluginloader.h"
#include "simpleprotocolparser.h"

namespace Core{

ProtocolManager* ProtocolManager::m_instance = NULL;

ProtocolManager *ProtocolManager::instance()
{
    if(m_instance == NULL)
        m_instance = new ProtocolManager();
    return m_instance;
}


ProtocolManager::ProtocolManager()
{

}

/*!
 * @brief 解析指定文件夹下的xml文件
 * @param[in] dirname 文件夹名
 * @return true 全部解析成功 false 存在解析失败的，已经跳过解析失败的文件
 */
bool ProtocolManager::parseXmlInLocalDir(QString dirname,ProtocolParser &parser)
{
    QDir dir(dirname);
    if(!dir.exists())
        return false;

    QStringList filters;
    filters<<"*.xml";
    QFileInfoList fileList = dir.entryInfoList(filters);

    std::for_each(fileList.begin(),fileList.end(),[&](QFileInfo fileInfo){
        if(!parseLocalProtocol(fileInfo.absoluteFilePath(),parser))
            return false;
        return true;
    });
    return true;
}

/*!
 * @brief 解析指定的配置文件
 * @param[in] fullFilePath 待解析文件的全路径
 * @param[in] parser 指定协议解析器
 * @return true 解析成功 false 解析失败
 */
bool ProtocolManager::parseLocalProtocol(QString fullFilePath,ProtocolParser &parser)
{
    Base::RXmlFile xmlFile(fullFilePath);
    xmlFile.setParseMethod(&parser,false);
    if(xmlFile.startParse()){
        Core::BaseProtocol * bprotocol = parser.getProtocol();
        bprotocol->m_localXmlFullPath = fullFilePath;
        protocols.insert(bprotocol->name,bprotocol);
        return true;
    }
    return false;
}

/*!
 * @brief 统计指定类型协议数量
 * @attention 20190409:wey:调整对多个相同的协议解析(通过配置时绑定协议ip地址)
 * @param[in] protocolType 协议类型
 * @return 指定类型协议数量
 */
int ProtocolManager::protocolCount(const QString & networId,int protocolType)
{
    int t_size = 0;
    BaseProtocolMap::iterator iter = protocols.begin();
    while(iter != protocols.end()){
        if(iter.value()->type == protocolType && iter.value()->m_recvNetworkId == networId){
            t_size++;
        }
        iter++;
    }

    return t_size;
}

/*!
 * @brief 判断协议类型是否存在
 * @param[in] protocolType 协议类型
 * @return true 存在；false 不存在
 */
bool ProtocolManager::isExisted(const QString &networId, int protocolType)
{
    BaseProtocolMap::iterator iter = protocols.begin();
    while(iter != protocols.end()){
        if(iter.value()->type == protocolType && iter.value()->m_recvNetworkId == networId){
            return true;
            break;
        }
        iter++;
    }

    return false;
}

/*!
 * @brief 批量更新网卡对应的xml协议中ip属性
 * @details 1.更新网卡为tcp时，将上位机ip和port更新分别更新到plugins.xml中的对应<network>节点中的<ip>和<port>节点；以及此网卡下所有xml文件中<protocol>节点的ip属性
 *          2.更新网卡为udp时，将设备ip和port分别更新至plugins.xml中的对应<network>节点中的<ip>和<port>节点。
 * @param[in] list 待更新网卡数据集合
 */
void ProtocolManager::updateProtocolIp(DeviceNetworkUpdateList *list)
{
    if(list){
        bool t_bNeedWriteToDisk = false;
        for(int i = 0; i < list->size(); i++){
            const DeviceNetworkUpdateInfo item = list->at(i);
            if(item.m_networkType == Core::Datastruct::N_TCP){
                QStringList localXmlPathList = getXmlsFullPath(item.m_systemNetworkId);
                if(localXmlPathList.size() > 0){
                    NetworkMap * networkMap = PluginLoader::instance()->getNetworks();
                    if(networkMap){

                        //更新plugins.xml文件
                        if(!item.m_systemListenIp.isEmpty()){
                            Datastruct::NetworkInfo & currNetInfo = networkMap->operator [](item.m_systemNetworkId);
                            currNetInfo.baseInfo.ip = item.m_systemListenIp;
                            currNetInfo.baseInfo.port = item.m_systemListenPort;
                            t_bNeedWriteToDisk = true;
                        }

                        //更新对应协议的ip属性
                        for(int j = 0; j < localXmlPathList.size(); j++){
                            SimpleProtocolParser parser(localXmlPathList.at(j));
                            parser.setProtocolAttribute(item.m_protocolAttributesMap);
                            if(!parser.startSave()){
                                qDebug()<<"update protcol failed!";
                            }
                        }
                    }
                }
            }else{
                NetworkMap * networkMap = PluginLoader::instance()->getNetworks();
                if(networkMap){
                    Datastruct::NetworkInfo & currNetInfo = networkMap->operator [](item.m_systemNetworkId);
                    currNetInfo.baseInfo.ip = item.m_deviceListenIp;
                    currNetInfo.baseInfo.port = item.m_deviceListenPort;
                    t_bNeedWriteToDisk = true;
                }
            }
        }

        if(t_bNeedWriteToDisk){
            PluginLoader::instance()->saveModuleFile();
        }
    }
}

/*!
 * @brief 获取指定网卡下对应协议xml的完整路径信息
 * @date 2019-10-28
 * @param[in] networkId 网卡唯一标识
 * @return 对应网卡下xml文件全路径集合
 */
QStringList ProtocolManager::getXmlsFullPath(QString networkId)
{
    QStringList t_result;
    QMapIterator<QString,Core::BaseProtocol*> t_iter(protocols);
    while(t_iter.hasNext()){
        t_iter.next();
        if(t_iter.value()->m_recvNetworkId == networkId){
            t_result.append(t_iter.value()->m_localXmlFullPath);
        }
    }

    return t_result;
}

/*!
 * @brief 根据协议名称获取对应的协议
 * @attention 协议名可作为唯一访问标志
 * @param[in]  protocolName 协议名
 * @param[in/out]  existed true：对应的协议是否存在;false：对应的协议不存在
 * @return 协议模板
 */
const Core::BaseProtocol* ProtocolManager::getProtocol(QString protocolName, bool *existed)
{
    *existed = protocols.contains(protocolName);
    if(*existed)
        return (protocols.value(protocolName));

    return nullptr;
}

/*!
 * @brief 根据协议名称获取对应的协议
 * @param[in]  protocolType 协议类型,如0xCCDD
 * @param[in/out]  existed true：对应的协议是否存在;false：对应的协议不存在
 * @return 协议模板
 */
const Core::BaseProtocol* ProtocolManager::getProtocol(const QString &networId,int protocolType,bool *existed)
{
    BaseProtocolMap::iterator iter = protocols.begin();
    while(iter != protocols.end()){
        if(iter.value()->type == protocolType && iter.value()->m_recvNetworkId == networId){
            *existed = true;
            return (iter.value());
        }
        iter++;
    }

    return nullptr;
}

/*!
 * @brief 根据协议名称获取对应的协议
 * @param[in]  protocolType 协议类型,如0xCCDD
 * @param[in]  ip 协议绑定的ip地址
 * @param[in/out]  existed true：对应的协议是否存在;false：对应的协议不存在
 * @return 协议模板
 */
const BaseProtocol *ProtocolManager::getProtocol(const QString &networId,int protocolType, QString ip, bool *existed)
{
    BaseProtocolMap::iterator iter = protocols.begin();
    while(iter != protocols.end()){
        if(iter.value()->type == protocolType && iter.value()->m_recvNetworkId == networId){
            if(ip.isEmpty()){
                *existed = true;
                return (iter.value());
            }else{
                if(iter.value()->m_ip == ip){
                    *existed = true;
                    return (iter.value());
                }
            }
        }
        iter++;
    }

    return nullptr;
}

}
