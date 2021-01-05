/*!
 *  @brief     转发规则文件解析方式
 *  @details   按行读取规则，每行使用:分割，格式如“接收协议类型:本机Ip：本机port:发送协议类型:远程Ip:远程port”
 *  @author    wey
 *  @version   1.0
 *  @date      2019.04.17
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef RULEPARSEMETHOD_H
#define RULEPARSEMETHOD_H

#include <base/util/fileutils.h>
#include <core/protocol/datastruct.h>

namespace Related{

class RuleParseMethod : public Base::RTextParseMethod
{
public:
    RuleParseMethod();
    ~RuleParseMethod();

    bool  startParse(Base::RTextFile * file) override ;
    bool  startSave(Base::RTextFile * file) override;

    Core::Datastruct::ForwardList getForwardList(){return m_fowardList;}

private:
    enum RulePos{
        RecvProtocol = 0x00,
        RecvIp,
        RecvPort,
        RemotePortocol,
        RemoteIp,
        RemotePort,
        RuleSize
    };

private:
    Core::Datastruct::ForwardList m_fowardList;

};

} //namespace Related

#endif // RULEPARSEMETHOD_H
