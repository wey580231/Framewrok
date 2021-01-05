/*!
 *  @brief     数据转换管理
 *  @details   1.解析转换规则；
 *             2.分配转发规则
 *  @author    wey
 *  @version   1.0
 *  @date      2019.04.17
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef DATAFORWARDMANAGER_H
#define DATAFORWARDMANAGER_H

#include <mutex>
#include <core/protocol/datastruct.h>
#include <related/related_global.h>

namespace Related{

class DataForwardManager
{
public:
    static DataForwardManager * instance();
    ~DataForwardManager();

    void parseForwardRule();

    Core::Datastruct::ForwardList getForwardRule();
    const Core::Datastruct::ForwardConfigInfo  findForwardRule(QString id);
	
private:
    explicit DataForwardManager();

private:
    static DataForwardManager * m_instance;
    Core::Datastruct::ForwardList m_forwardList;

};

} //namespace Related

#endif // DATAFORWARDMANAGER_H
