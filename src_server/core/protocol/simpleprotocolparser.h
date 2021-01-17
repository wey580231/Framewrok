/*!
 *  @brief     轻量级xml协议解析器
 *  @details   只解析<protocol>节点，用来更新相关的属性值。包括更新ip、recvNet等
 *  @attention
 *  @author    wey
 *  @version   1.0
 *  @date      2019.10.28
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef SIMPLEPROTOCOLPARSER_H
#define SIMPLEPROTOCOLPARSER_H

#include <QMap>
#include <base/util/fileutils.h>

namespace Core {

class SimpleProtocolParser
{
public:
    explicit SimpleProtocolParser(QString fileName);

    void setProtocolAttribute(QMap<QString, QString> attributes);

    bool startSave();

private:
    QMap<QString,QString> m_updateProtocolAttributes;       /*!< 待更新<protocol>节点属性集合 */

    QString m_fullFileName;
};

} //namespace Core

#endif // SIMPLEPROTOCOLPARSER_H
