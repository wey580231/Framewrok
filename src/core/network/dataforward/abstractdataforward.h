/*!
 *  @brief     通用数据转发入口
 *  @details   不同的课题根据自己的需要去对数据进行转发处理，避免在core插件中过多的加入数据转发逻辑。
 *  @author    wey
 *  @version   1.0
 *  @date      2020.06.02
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef ABSTRACTDATAFORWARD_H
#define ABSTRACTDATAFORWARD_H

#include <list>
#include "../../core_global.h"
#include "../../protocol/datastruct.h"

namespace Core{

class CORESHARED_EXPORT AbstractDataForward : public QObject
{
    Q_OBJECT
public:
    AbstractDataForward();

    virtual void forward(std::list<Datastruct::ProtocolArray> & dataArrays) = 0;

};

class CORESHARED_EXPORT DataForwardManager
{
public:
    static DataForwardManager * instance();

    void registForward(AbstractDataForward * forward);

    void forward(std::list<Datastruct::ProtocolArray> & dataArrays);

private:
    DataForwardManager();

private:
    static DataForwardManager * m_instance;

    std::list<AbstractDataForward *> forwardList;
};

} //namespace Core

#endif // ABSTRACTDATAFORWARD_H
