#include "abstractflowcomponent.h"

#include <base/util/rutil.h>

namespace Core{

AbstractFlowComponent::AbstractFlowComponent(QWidget *parent):Widget(parent)
{
    m_uuid = Base::RUtil::UUID();
}

AbstractFlowComponent::~AbstractFlowComponent()
{

}

QString AbstractFlowComponent::id()
{
    return m_uuid;
}

} //namespace Core
