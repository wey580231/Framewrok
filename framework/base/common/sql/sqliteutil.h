﻿                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     /*!
*  @brief     sqlite工具类
*  @file      提供访问SQLite访问接口
*  @author    wey
*  @version   1.0
*  @date      2020.10.14
*  @warning
*  @copyright NanJing RenGu.
*  @note
*/
#ifndef SQLUTIL_H
#define SQLUTIL_H

#include <functional>
#include <QSqlDatabase>

#include "../../base_global.h"

namespace Base{

typedef std::function<void(QSqlQuery &)> QueryCallback;

class BASESHARED_EXPORT SqliteUtil
{
public:
   SqliteUtil();

   bool open(QString dbPath);
   bool isOpen() const {return m_sqlDatabase.isOpen();}

   bool createTable(QString sql);
   bool query(QString sql, QueryCallback callback = nullptr);

   QString lastError() const {return m_errorInfo;}

private:
   QSqlDatabase m_sqlDatabase;
   QString m_errorInfo;

};

} //namespace Base

#endif // SQLUTIL_H
