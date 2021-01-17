#include "rapplication.h"

#include <QDebug>

namespace Core {

RApplication::RApplication(int &argc, char **argv):QApplication(argc,argv)
{

}

RApplication::~RApplication()
{

}

bool RApplication::notify(QObject *obj, QEvent *event)
{
//    qDebug()<<"notify:"<<obj<<event->type();

    return QApplication::notify(obj,event);
}

} //namespace Core
