#ifndef JSJSONUTILS_H
#define JSJSONUTILS_H

#include <QString>

namespace Core{

namespace MapWeb2D {

class jsJsonObjectHelper
{
public:
    jsJsonObjectHelper(){
        m_szJsJson = "\'{";
    }

    void addData(QString szKey, QString szValue){
        m_szJsJson += QString("\"%1\":\"%2\",").arg(szKey).arg(szValue);
    }
    void addData(QString szKey, int dValue){
        m_szJsJson += QString("\"%1\":%2,").arg(szKey).arg(dValue);
    }
    void addData(QString szKey, bool bValue){
        m_szJsJson += QString("\"%1\":%2,").arg(szKey).arg(bValue?"true":"false");
    }

    QString toJsonObject(){
        QString t_szJsJson = m_szJsJson.left(m_szJsJson.size()-1); //去掉最后的逗号
        t_szJsJson += "}\'";
        return t_szJsJson;
    }

private:
    QString m_szJsJson;
};

} // namespace Core

} //namespace Core
#endif // JSJSONUTILS_H
