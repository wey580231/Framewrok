#include "abstractofficemenuitem.h"

#include <QWidget>

namespace Core{

AbstractOfficeMenuItem::AbstractOfficeMenuItem(QObject* parent):QObject(parent)
{
    m_container = new QWidget;
//    m_container->setObjectName("mainWidget");
//    m_container->setFocusPolicy(Qt::NoFocus);
}

AbstractOfficeMenuItem::~AbstractOfficeMenuItem()
{

}

} //namespace Core
