#include "binaryfilerecord.h"

#include <QDir>
#include <QFile>
#include <condition_variable>
#include <QDebug>

#include <base/util/rsingleton.h>
#include <base/util/rutil.h>
#include <core/pluginmanager/plugin/pluginloader.h>

namespace Core{

#define M_BYTES(n) (n * 1024 * 1024)

BinaryFileRecord::BinaryFileRecord()
{

}

BinaryFileRecord::~BinaryFileRecord()
{
    RecordFileMap::iterator iter = recordFilesMap.begin();
    while(iter != recordFilesMap.end()){
        if(iter.value()->localFile != NULL){
            if(iter.value()->localFile->isOpen())
                iter.value()->localFile->close();
            delete iter.value()->localFile;
        }
        delete iter.value();
        iter = recordFilesMap.erase(iter);
    }
}

/*!
 * @brief 添加新网卡至数据记录
 * @details 文件创建延迟至第一次数记录前
 * @param[in]  networkId 网卡ID
 */
void BinaryFileRecord::addNetworkInterface(QString networkId)
{
    std::lock_guard<std::mutex> lg(mapMutex);
    RecordingDesc * networkDesc = recordFilesMap.value(networkId);
    if(networkDesc == NULL){
        Core::NetworkMap * networkMap = Core::PluginLoader::instance()->getNetworks();
        if(networkMap->contains(networkId)){
            Datastruct::NetworkInfo netinfo = networkMap->value(networkId);
            networkDesc = new RecordingDesc;
            networkDesc->error = !isFileDirExisted(netinfo.baseInfo.recordNetPath);
            networkDesc->fileDir = netinfo.baseInfo.recordNetPath;
            networkDesc->recordFileSize = netinfo.baseInfo.singleFileSize;
            networkDesc->enableRecording = netinfo.baseInfo.recordNetFile;
            recordFilesMap.insert(networkId,networkDesc);
        }
    }
}

/*!
 * @brief 更新网卡状态信息
 * @details 此操作最好在程序记录前修改，程序运行过程修改，可能导致策略不能及时更新
 * @attention 此操作只会更新或减少网卡信息，不会新增网卡信息
 * @param[in] netmaps 新网卡信息
 */
void BinaryFileRecord::updateNetworkInterface(NetworkMap * netmaps)
{
    std::lock_guard<std::mutex> lg(mapMutex);
    RecordFileMap::iterator iter = recordFilesMap.begin();
    while(iter != recordFilesMap.end()){
        RecordingDesc * rdesc = iter.value();
        if(netmaps->contains(iter.key())){
            Datastruct::NetworkInfo newInfo = netmaps->value(iter.key());
            rdesc->enableRecording = newInfo.baseInfo.recordNetFile;
            rdesc->fileDir = newInfo.baseInfo.recordNetPath;
            rdesc->recordFileSize = newInfo.baseInfo.singleFileSize;

            rdesc->error = !isFileDirExisted(newInfo.baseInfo.recordNetPath);
            if(rdesc->error){
                if(rdesc->localFile)
                    delete rdesc->localFile;
                rdesc->localFile = NULL;
            }
        }else{
            if(rdesc->localFile){
                if(rdesc->localFile->isOpen())
                    rdesc->localFile->close();
                delete rdesc->localFile;
            }
            delete rdesc;
            iter = recordFilesMap.erase(iter);
        }
        iter++;
    }
}
\
/*!
 * @brief 移除指定网卡至数据记录
 * @param[in]  networkId 网卡ID
 */
void BinaryFileRecord::removeNetworkInterface(QString networkId)
{
    std::lock_guard<std::mutex> lg(mapMutex);
    RecordingDesc * networkDesc = recordFilesMap.value(networkId);
    if(networkDesc != NULL){
        if(networkDesc->localFile){
            if(networkDesc->localFile->isOpen())
                networkDesc->localFile->close();
            delete networkDesc->localFile;
        }
        delete networkDesc;
        recordFilesMap.remove(networkId);
    }
}

/*!
 * @brief 控制指定网卡是否数据记录
 * @param[in] networkId 待设置网卡Id
 * @param[in] enabled 是否开启数据记录
 */
void BinaryFileRecord::enableNetworkInterface(QString networkId, bool enabled)
{
    std::lock_guard<std::mutex> lg(mapMutex);
    RecordingDesc * networkDesc = recordFilesMap.value(networkId);
    if(networkDesc != NULL){
        networkDesc->enableRecording = enabled;
    }
}

/*!
 * @brief 将数据记录至文件
 * @details 1.根据数据所属网卡查找是否存在记录文件;
 *          2.写入数据信息;
 *          3.若文件超过文件块限制，关闭文件并创建新文件。
 * @param[in]  array 原始块数据信息
 */
void BinaryFileRecord::record(Datastruct::ProtocolArray &array)
{
    std::lock_guard<std::mutex> lg(mapMutex);
    RecordingDesc * networkDesc = recordFilesMap.value(array.networkId);

    if(networkDesc != NULL && !networkDesc->error && networkDesc->enableRecording){
        if(networkDesc->localFile == NULL){
            QFile * recordFile = createFile(networkDesc,array.networkId,array.ip);
            if(recordFile == NULL){
                networkDesc->error = true;
                return;
            }else{
                networkDesc->localFile = recordFile;
            }
        }

        int writeBytes = networkDesc->localFile->write(*array.data);
        if(writeBytes >= 0){
            networkDesc->localFile->flush();
        }else{
            networkDesc->localFile->close();
            networkDesc->error = true;
            return;
        }

        if(networkDesc->localFile->size() > M_BYTES(networkDesc->recordFileSize)){
            networkDesc->localFile->close();
            delete networkDesc->localFile;

            networkDesc->localFile = createFile(networkDesc,array.networkId,array.ip);

            if(networkDesc->localFile == NULL){
                networkDesc->error = true;
                delete networkDesc->localFile ;
            }
        }
    }
}

bool BinaryFileRecord::isFileDirExisted(QString filePath)
{
    QDir dir(filePath);
    return dir.exists();
}

/*!
* @brief 根据网卡创建唯一文件名
* @details 文件名按照“网卡ID_发送端IP_时间戳_自增索引.bin”
* @warning 【ip地址的分割符与数据回放处需要保持一致，否则会解析出错】
* @param[in] rdesc 文件记录信息
* @param[in] networkdId 网卡ID
* @param[in] srcIp 发送端IP地址
*/
QFile * BinaryFileRecord::createFile(RecordingDesc * rdesc,QString networkdId,QString srcIp)
{
    if(Base::RUtil::createDir(rdesc->fileDir)){
        //TODO 20190112 根据名称策略二进制创建文件
        QString t_datetime;
        if(rdesc->baseFileName.isEmpty()){
            t_datetime = QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
        }
        QString t_name = QString("%1_%2_%3_%4.bin");
        rdesc->baseFileName = t_name.arg(networkdId).arg(srcIp.replace(QRegExp("\\."),"-")).arg(t_datetime).arg(rdesc->index++);
        QString fullPath = rdesc->fileDir + QDir::separator() + rdesc->baseFileName;
        QFile * file = new QFile(fullPath);
        if(!file->open(QFile::WriteOnly)){
            delete file;
        }else{
            return file;
        }
    }

    return NULL;
}

} //namespace Core
