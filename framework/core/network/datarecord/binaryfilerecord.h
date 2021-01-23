/*!
 *  @brief     TCP二进制文件记录
 *  @details   1.添加、删除、使能指定记录网卡;
 *             2.记录文件时需要参照记录策略(文件大小、文件名等);
 *             3.数据记录只在实时工作模式下，回放时数据不记录
 *  @author    wey
 *  @version   1.0
 *  @date      2019.01.12
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 *      20190606:wey:调整文件名为“网络ID_发放主机IP_时间戳.bin”
 */
#ifndef BINARYFILERECORD_H
#define BINARYFILERECORD_H

#include "filerecord.h"
#include <core/protocol/datastruct.h>
#include <core/core_global.h>

#include <QList>
#include <QMap>

#include <mutex>

class QFile;

namespace Core{

using namespace Datastruct;

class CORESHARED_EXPORT BinaryFileRecord : public FileRecord
{
public:
    explicit BinaryFileRecord();
    ~BinaryFileRecord();

    void addNetworkInterface(QString networkId);
    void updateNetworkInterface(Core::NetworkMap *netmaps);
    void removeNetworkInterface(QString networkId);
    void enableNetworkInterface(QString networkId,bool enabled = true);

    void record(Datastruct::ProtocolArray & array);

    struct RecordingDesc{
        RecordingDesc():error(false),index(1),enableRecording(false),recordFileSize(-1),localFile(NULL){}
        bool error;
        QString baseFileName;
        int index;                  /*!< 文件分片索引 */
        QString fileDir;            /*!< 保存的文件夹 */
        bool enableRecording;       /*!< 是否开启记录 */
        int recordFileSize;         /*!< 单个文件记录大小(M),超过此限制后，自动创建新的文件 */
        QFile * localFile;          /*!< 文件句柄 */
    };

private:
    bool isFileDirExisted(QString filePath);
    QFile *createFile(RecordingDesc *rdesc, QString networkdId, QString srcIp);

private:
    typedef QHash<QString,RecordingDesc * > RecordFileMap;   /*!< 文件记录映射，key:网卡id;value:文件句柄 */

    RecordFileMap recordFilesMap;

    std::mutex mapMutex;

};

} //namespace Core

#endif // BINARYFILERECORD_H
