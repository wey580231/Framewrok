#ifndef SOCKHEAD_H
#define SOCKHEAD_H

#include <QString>
#include <QDebug>

#include "../network_global.h"

namespace Network{

struct NETWORKSHARED_EXPORT NetAddress{
    NetAddress(){}
    NetAddress(const char * _ip,ushort _port):ip(_ip),port(_port){}
    NetAddress(QString _ip,ushort _port):ip(_ip),port(_port){}

    NetAddress(const NetAddress & otherAddress){
        ip = otherAddress.ip;
        port = otherAddress.port;
    }

    void operator= (const NetAddress & otherAddress){
        ip = otherAddress.ip;
        port = otherAddress.port;
    }

    bool operator==(const NetAddress & addr){
        if(this == &addr)
            return true;
        return this->ip == addr.ip && this->port == addr.port;
    }

    friend NETWORKSHARED_EXPORT QDebug & operator<<(QDebug & data,const NetAddress & addr);

    QString ip;
    ushort port;
};

} //namespace Network

#endif // SOCKHEAD_H
