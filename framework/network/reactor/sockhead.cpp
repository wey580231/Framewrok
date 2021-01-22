#include "sockhead.h"

namespace Network{

QDebug & operator<<(QDebug & data,const NetAddress & addr){
    data<<"["<<addr.ip<<":"<<addr.port<<"]";
    return data;
}

}
