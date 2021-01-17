/*!
 *  @brief     协议解析线程
 *  @details   1.处理网络线程打包好的原始协议数据；
 *             2.将解析后的数据分发给不同的插件；
 *  @author    wey
 *  @version   1.0
 *  @date      2018.11.08
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 *      20190112:wey:增加对TCP数据文件记录
 *      20190313:yww:解析线程一次性读取尽可能多的数据处理，减少加锁解锁频率
 *      20190417:wey:增加铁塔前端数据转发(正常工作模式)
 *      20190914：yww 当插件需要对当前的解析线程进行注册网络协议时，即使使用Rsinlegleton<ProtocolParseThread>
 *              也会导致每一个动态库中存在一个Rsinlegleton<ProtocolParseThread>。不能共享interstedProtocol；
 *              特地改成static类型
 */
#ifndef PROTOCOLPARSETHREAD_H
#define PROTOCOLPARSETHREAD_H

#include "../rtask.h"

#include "core/protocol/datastruct.h"
#include "core/protocol/waitimplemneted.h"
#include "core/core_global.h"
#include "core/global.h"

#include <QHash>
class QFile;
class QTimer;

namespace Core{

namespace NetParse{
class DataGenertor;
}

class BinaryFileRecord;

using namespace RGlobal;

class CORESHARED_EXPORT ProtocolParseThread : public RTask
{
    Q_OBJECT
public:
    explicit ProtocolParseThread();
    ~ProtocolParseThread();

    void registNetworkObserver(QString moduleid, const QString &networkId, QStringList protos, NetworkType ntype = Datastruct::N_TCP);

    void startMe();
    void stopMe();

    BinaryFileRecord * getFileRecord(){return fileRecord;}

    struct ModuleProtocol{
        NetworkType ntype;          /*!< 网络类型 */
        QString moduleId;           /*!< 模块Id,组件的objectName */
    };

    typedef QList<ModuleProtocol> MProtocolList;
    ProtocolParseThread::MProtocolList * getProtocolObservers(const QString & networkId,int protocolType);

    void modifyParseLimit(ParseLimitList *list);

signals:
    void protocolDataReady();

protected:
    void run();

private:
    bool beforeParsing(Datastruct::ProtocolArray & array, std::list<ParserResult *> &buff);
private:
    std::mutex pmutex;

    static QHash<QString,MProtocolList *> interstedProtocol;   /*!< key:networkId_protoId,value:订阅此信息插件集合 */

    BinaryFileRecord * fileRecord;
    QTimer * m_testTimer;

    QMap<int,Datastruct::ProtocolParseLimit>    m_protocolLimitMap;     /*!< 协议解析时限制条件 */
};

}

#endif // PROTOCOLPARSETHREAD_H
