#include "simpleprotocolparser.h"

#include <base/util/rlog.h>
#include <QFile>
#include <QDebug>
#include <QTextCodec>

namespace Core {

SimpleProtocolParser::SimpleProtocolParser(QString fileName):m_fullFileName(fileName)
{

}

void SimpleProtocolParser::setProtocolAttribute(QMap<QString,QString> attributes)
{
    m_updateProtocolAttributes = attributes;
}

bool SimpleProtocolParser::startSave()
{
    Base::RFile tfile(m_fullFileName);
    if(tfile.startParse()){
        QDomDocument doc;
        QString errorMsg;
        int errorRow = 0,errorCol = 0;

        if(!doc.setContent(&tfile, false, &errorMsg, &errorRow, &errorCol)){
            RLOG_INFO("Open xml file error [row:%d,col:%d,msg:%s]!",errorRow,errorCol,errorMsg.toLocal8Bit().data());
            return false;
        }

        tfile.close();

        QDomElement root = doc.documentElement();
        if(!root.isNull())
        {
            //更新节点配置
            QMapIterator<QString,QString> t_mapIter(m_updateProtocolAttributes);
            while(t_mapIter.hasNext()){
                t_mapIter.next();
                root.setAttribute(t_mapIter.key(),t_mapIter.value());
            }

            Base::RFile tsaveFile(m_fullFileName);
            if(tsaveFile.startSave()){
                QTextStream stream(&tsaveFile);
                stream.setCodec(QTextCodec::codecForLocale());

                QDomDocument doc("");

                QDomProcessingInstruction instruction = doc.createProcessingInstruction("xml","version='1.0' encoding='UTF-8'");
                doc.appendChild(instruction);

                doc.appendChild(root);

                doc.save(stream,4);
                return true;
            }
        }
    }

    return false;
}

} //namespace Core
