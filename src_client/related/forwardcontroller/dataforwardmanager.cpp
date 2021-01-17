#include "dataforwardmanager.h"

#include <QFile>
#include <base/util/rlog.h>
#include <base/util/fileutils.h>
#include "ruleparsemethod.h"
#include <core/file/programfilepath.h>

namespace Related{

DataForwardManager * DataForwardManager::m_instance = nullptr;

std::once_flag m_onceForward;

DataForwardManager::DataForwardManager()
{

}

DataForwardManager *DataForwardManager::instance()
{
    std::call_once(m_onceForward,[&](){m_instance = new DataForwardManager();});
    return m_instance;
}

DataForwardManager::~DataForwardManager()
{

}

/*!
 * @brief 解析转发规则
 * @details 转发规则记录表按照“接收协议类型:本地ip:本地端口:发送协议类型:远程ip:远程端口”，
 *        按行记录，每一行代表一条转发规则
 */
void DataForwardManager::parseForwardRule()
{
//    Core::ProgramFilePath filePath;
//    Base::RTextFile t_textFile(filePath.forwardFile);
//    RuleParseMethod * t_method = new RuleParseMethod();
//    t_textFile.setParseMethod(t_method);
//    if(t_textFile.startParse()){
//        m_forwardList = t_method->getForwardList();
//    }else{
//        RLOG_ERROR("forward rule is empty!");
//    }
}

Core::Datastruct::ForwardList DataForwardManager::getForwardRule()
{
    return this->m_forwardList;
}

const Core::Datastruct::ForwardConfigInfo DataForwardManager::findForwardRule(QString id)
{
    QList<Core::Datastruct::ForwardConfigInfo>::iterator ite = m_forwardList.begin();
    for(;ite != m_forwardList.end();ite++) {
        if(ite->id.contains(id))
            return *ite;
    }
}

} //namespace Related
