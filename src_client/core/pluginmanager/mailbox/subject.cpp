#include "subject.h"

#include "observer.h"

namespace Core{

Subject::Subject()
{

}

Subject * Subject::m_instance = nullptr;

Subject *Subject::instance()
{
    if(m_instance == nullptr)
        m_instance = new Subject();
    return m_instance;
}

void Subject::attach(Observer *obj)
{
    std::lock_guard<std::mutex> lg(m_mutex);
    observers.push_back(obj);
}

void Subject::detach(Observer *obj)
{
    std::lock_guard<std::mutex> lg(m_mutex);
    observers.remove(obj);
}

void Subject::notify(QString messageType)
{
    std::lock_guard<std::mutex> lg(m_mutex);

    std::list<Observer *>::iterator iter = observers.begin();
    while(iter != observers.end())
    {
        (*iter)->onMessage(messageType);
        iter++;
    }
}

} //namespace Base
