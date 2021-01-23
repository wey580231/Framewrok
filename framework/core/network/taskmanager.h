/*!
 *  @brief     任务管理
 *  @details
 *  @author    wey
 *  @version   1.0
 *  @date      2018.11.12
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include <QMap>
#include <memory>
#include <mutex>

#include "../core_global.h"

namespace Core{

class RTask;
typedef std::shared_ptr<RTask> TaskPtr;

class CORESHARED_EXPORT TaskManager
{
public:
    static TaskManager * instance();
    void addTask(QString id, TaskPtr ptr);
    TaskPtr getTask(QString id);

    void releaseTask();

private:
    TaskManager();
    static TaskManager * m_instance;

    typedef QMap<QString,std::shared_ptr<RTask>> TaskMap;

    std::mutex t_mutex;
    TaskMap taskMap;

};

}

#endif // TASKMANAGER_H
