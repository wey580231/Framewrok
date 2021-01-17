#include "abstractdataforward.h"

namespace Core{

AbstractDataForward::AbstractDataForward()
{

}

DataForwardManager::DataForwardManager()
{

}

DataForwardManager * DataForwardManager::m_instance = nullptr;

DataForwardManager *DataForwardManager::instance()
{
    if(m_instance == nullptr){
        m_instance = new DataForwardManager();
    }
    return m_instance;
}

void DataForwardManager::registForward(AbstractDataForward *forward)
{
    if(forward == nullptr)
        return;

    forwardList.push_back(forward);
}

void DataForwardManager::forward(std::list<Datastruct::ProtocolArray> &dataArrays)
{
    if(forwardList.size() == 0)
        return;

    std::for_each(forwardList.begin(),forwardList.end(),[&](AbstractDataForward * forwardClient){
        forwardClient->forward(dataArrays);
    });
}

} //namespace Core
