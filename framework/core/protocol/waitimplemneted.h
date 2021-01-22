#ifndef WAITIMPLEMNETED_H
#define WAITIMPLEMNETED_H

#include <base/util/fileutils.h>
#include <core/core_global.h>
#include "./datastruct.h"

namespace Core {
using namespace Datastruct;

class DataParser;
class ParserResult;

/*!
 *  @brief 基本协议
 *  @details 适应当前项目的xml协议内容
 */
class CORESHARED_EXPORT BaseProtocol{
public:
    explicit BaseProtocol(DataParser *parser);
    DataParser* getParser();
    virtual ~BaseProtocol(){}

    QString name;       /*!< 协议名 */
    int type;           /*!< 协议类型 */

    int startCode;      /*!< 开始标志码 */
    int startLen;       /*!< 数据头部固定长度 */
    int endCode;        /*!< 结束标志码 */
    int endLen;         /*!< 数据尾部固定长度 */

    QString m_ip;       /*!< 此协议从指定IP发送过来 20190409:wey */
    QString m_recvNetworkId;        /*!< 协议绑定接收网卡ID 20190410:wey */
    QString m_localXmlFullPath;     /*!< 本地完整协议路径名 20191028:wey */
private:
    DataParser *m_parser;
};

class CORESHARED_EXPORT ProtocolParser : public Base::RXmlParseMethod{

public:
    BaseProtocol* getProtocol(){return parsedProtocol;}

protected:
    BaseProtocol *parsedProtocol;
};
/*!
 *  @brief 数据解析器
 *  @details 对应当前项目的xml文件的解析方法
 */
class CORESHARED_EXPORT DataParser{
public:
    explicit DataParser();
    void setBaseProtocol(BaseProtocol *baseProtocol);
    virtual ParserResult* startParse(ProtocolArray &array,Core::Datastruct::ProtocolParseLimit limit) = 0;
protected:
    BaseProtocol *m_baseProtocol;
};

/*!
 *  @brief 数据解析结果
 *  @details 数据线程解析后，发信号由页面线程处理
 */
class CORESHARED_EXPORT ParserResult{

public:
    virtual ~ParserResult(){}
    virtual int getProtocolType() = 0;

    void setProtocolIp(QString ip){m_protocolIp = ip;}
    QString getProtocolIp(){return this->m_protocolIp;}

    void setNetworkId(QString netid){m_networkId = netid;}
    QString getNetworkId(){return this->m_networkId;}

private:
    QString m_protocolIp;       /*!< 协议对应的Ip地址，可用来判断：20190409：wey */
    QString m_networkId;        /*!< 网卡id：20190410：wey */
};
}

#endif // WAITIMPLEMNETED_H
