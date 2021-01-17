#include "protocolparsethread.h"

#include <thread>
#include <iostream>
#include <QTimer>
#include <QDebug>

#include <base/util/rsingleton.h>
#include <base/util/rutil.h>

#include <core/protocol/protocolmanager.h>
#include <core/file/globalconfigfile.h>
#include <core/pluginmanager/plugin/pluginloader.h>
#include "../datarecord/binaryfilerecord.h"
#include "../dataforward/abstractdataforward.h"

namespace Core{

QHash<QString,ProtocolParseThread::MProtocolList *> ProtocolParseThread::interstedProtocol;

ProtocolParseThread::ProtocolParseThread()
{
    fileRecord = new BinaryFileRecord;

    Core::NetworkMap * networkMap = Core::PluginLoader::instance()->getNetworks();
    if(networkMap != nullptr){
        Core::NetworkMap::iterator niter = networkMap->begin();
        while(niter != networkMap->end()){
            if(niter.value().protocol == Datastruct::N_TCP){
                fileRecord->addNetworkInterface(niter.key());
            }
            niter++;
        }
    }
}

ProtocolParseThread::~ProtocolParseThread()
{
    runningFlag = false;
    G_ProtocolQueue.wakeUpConsumer();

    wait();
}

/*!
 * @brief 注册解析的网络数据协议
 * @param[in] moduleid   模块id
 * @param[in] networkId  网络模块id
 * @param[in] protos   此模块感兴趣的协议集合，为整形，与网络协议中数据类型字段保持一致
 * @param[in] ntype   网络通信链路方式
 */
void ProtocolParseThread::registNetworkObserver(QString moduleId,const QString & networkId, QStringList protos, NetworkType ntype)
{
    std::lock_guard<std::mutex> lg(pmutex);
    bool flag = false;
    std::for_each(protos.begin(),protos.end(),[&](QString protocol){
        int result = protocol.toInt(&flag,16);
        if(flag){
            QString key = QString("%1_%2").arg(networkId).arg(result);
            MProtocolList * list = interstedProtocol.value(key);
            if(list == nullptr){
                list = new MProtocolList;
                interstedProtocol.insert(key,list);
            }
            list->append({ntype,moduleId});
        }
    });
}

void ProtocolParseThread::startMe()
{
    RTask::startMe();
    runningFlag = true;
    start();
}

void ProtocolParseThread::stopMe()
{
    RTask::stopMe();
    runningFlag = false;
    G_ProtocolQueue.wakeUpConsumer(true);
}

/*!
 * @brief 获取订阅协议的组件信息
 *        修改将加锁与解锁的范围控制为一次
 * @param[in]  networkId 网卡ID
 * @param[in]  protocolType 协议id
 * @return 若存在，返回对应的链表；否则返回NULL
 */
ProtocolParseThread::MProtocolList *ProtocolParseThread::getProtocolObservers(const QString & networkId,int protocolType)
{
    QString key = QString("%1_%2").arg(networkId).arg(protocolType);
    return interstedProtocol.value(key);
}

void ProtocolParseThread::run()
{
    while(runningFlag){

        while(runningFlag && G_ProtocolQueue.size() == 0){
            G_ProtocolQueue.wait();
        }

        if(runningFlag){
            //[1]
            std::list<Datastruct::ProtocolArray> dataList = G_ProtocolQueue.takeAll();
            std::list<Core::ParserResult*> parsedDataList;

            //数据转发
            DataForwardManager::instance()->forward(dataList);

            //数据解析
            std::for_each(dataList.begin(),dataList.end(),[&](Datastruct::ProtocolArray &array){
                beforeParsing(array,parsedDataList);
            });

            RGlobal::G_ParsedProtocols.put(parsedDataList);

            emit protocolDataReady();
        }
    }

    delete fileRecord;
    fileRecord = NULL;
}

/*!
* @brief 更新协议解析限制
* @param[in] list 限制结合，同一个插件可能存在多个协议
*/
void ProtocolParseThread::modifyParseLimit(ParseLimitList * list)
{
    for(int i = 0; i < list->size(); i++){
        if(m_protocolLimitMap.contains(list->at(i).protocolType)){
            m_protocolLimitMap.operator [](list->at(i).protocolType) = list->at(i);
        }else{
            m_protocolLimitMap.insert(list->at(i).protocolType,list->at(i));
        }
    }
}

/*!
 * @brief 按照协议解析原始数据
 * @details 1.查找对应的网络协议数量，若=1可直接用协议模板解析；若>1需要结合ip地址和类型一并作为查找模板条件，不存在直接跳过返回；
 *          2.查找对此网络协议订阅的模块是否存在，不存在直接跳过返回；
 *          3.按照协议解析对应的字段；
 *          4.查找对应类型解析条数是否存在限制
 * @param[in] array 原始数据
 * @param[in] buff 解析结果缓冲列表
 * @return 是否解析成功
 */
bool ProtocolParseThread::beforeParsing(ProtocolArray &array,std::list<Core::ParserResult*> &buff)
{
    if(RGlobal::G_GlobalConfigFile->systemConfigInfo.m_bDebugNetParsedInfo){
        qDebug()<<"ProtocolParseThread: protocolType::"<<array.protocolType<<array.data->size();
    }

    //[0] 数据记录
    if(RGlobal::G_WorkState.mWorkMode == Datastruct::SystemWorkState::WORK_RealData){
        fileRecord->record(array);
    }

    //[1] 数据协议类型
    int t_i_count = ProtocolManager::instance()->protocolCount(array.networkId,array.protocolType);
    if(t_i_count <= 0)
        return false;

    //[2]
    MProtocolList * list = getProtocolObservers(array.networkId,array.protocolType);
    if(list == nullptr)
        return false;

    //[3]
    bool existed = false;
    Core::BaseProtocol *protocol = nullptr;
    if(t_i_count > 1)
       protocol = const_cast<Core::BaseProtocol*>(ProtocolManager::instance()->getProtocol(array.networkId,array.protocolType,array.ip,&existed));
    else
       protocol = const_cast<Core::BaseProtocol*>(ProtocolManager::instance()->getProtocol(array.networkId,array.protocolType,&existed));

    if(protocol == nullptr)
        return false;

    //[4]
    ProtocolParseLimit t_protoLimit = m_protocolLimitMap.value(array.protocolType);

    Core::ParserResult* result = protocol->getParser()->startParse(array,t_protoLimit);
    if(nullptr == result)
        return false;

    result->setProtocolIp(array.ip);
    result->setNetworkId(array.networkId);

    buff.push_back(result);

    return true;
}

} //namespace Core
