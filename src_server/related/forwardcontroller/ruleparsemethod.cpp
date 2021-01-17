#include "ruleparsemethod.h"

#include <QDebug>
#include <QStringList>

namespace Related{

RuleParseMethod::RuleParseMethod()
{

}

RuleParseMethod::~RuleParseMethod()
{

}

bool RuleParseMethod::startParse(Base::RTextFile *file)
{
    QByteArray t_lineBuffer;

    while((t_lineBuffer = file->readLine()).size() > 0){
        QString t_oneline(t_lineBuffer);
        QStringList t_originRuleList = t_oneline.split(':');
        if(t_originRuleList.size() == RuleSize +1){//多的一个字符为信息类别
            Core::Datastruct::ForwardConfigInfo t_rule;
            t_rule.m_localProtocol = t_originRuleList.at(RecvProtocol).toLower().contains("udp")?Core::Datastruct::N_UDP:Core::Datastruct::N_TCP;
            t_rule.m_localIp = t_originRuleList.at(RecvIp);
            t_rule.m_localPort = t_originRuleList.at(RecvPort).toUShort();

            t_rule.m_remoteProtocol = t_originRuleList.at(RemotePortocol).toLower().contains("udp")?Core::Datastruct::N_UDP:Core::Datastruct::N_TCP;
            t_rule.m_remoteIp = t_originRuleList.at(RemoteIp);
            t_rule.m_remotePort = t_originRuleList.at(RemotePort).toUShort();
            t_rule.id = t_originRuleList.last();
            m_fowardList.append(t_rule);
        }
    }

    return true;
}

bool RuleParseMethod::startSave(Base::RTextFile *file)
{

    return true;
}

} //namespace Related{
