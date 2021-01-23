#include "abstractmailbox.h"

namespace Core{

AbstractMailBox::AbstractMailBox(QObject *parent):RTask(parent)
{

}

AbstractMailBox::~AbstractMailBox()
{
    wait();
}

void AbstractMailBox::startMe()
{
    if(!isRunning()){
        runningFlag = true;
        start();
    }
}

void AbstractMailBox::stopMe()
{
    runningFlag = false;
    m_recvMailBox.wakeUpConsumer(true);
}

} //namespace Core
