#ifndef EASYQTSQL_DELETEQUERY_H
#define EASYQTSQL_DELETEQUERY_H

/*
 * The MIT License (MIT)
 * Copyright 2018 Alexey Kramin
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
*/

#ifndef EASY_QT_SQL_MAIN

#include <QtSql>
#include "EasyQtSql_NonQueryResult.h"

#endif

/*!
\brief QSqlQuery wrapper for <em>DELETE FROM ... WHERE ..</em> query execution.
*/
class  DeleteQuery
{
public:
   DeleteQuery(const QString &table, const QSqlDatabase &db)
     : m_query(db)
     , m_table(table)
   {   }

   /*!
   \brief Executes conditional <em>DELETE FROM table WHERE expr</em> query

   \code
   //DELETE FROM table WHERE 1=0
   t.deleteFrom("table").where("1=0");
   \endcode
   */
   NonQueryResult where(const QString &expr)
   {
      const QString &sql = createSql(m_table, expr);

      const bool res = m_query.exec(sql);

#ifdef DB_EXCEPTIONS_ENABLED

      if (!res)
         throw DBException(m_query);

#endif

      return NonQueryResult(m_query);
   }

   /*!
   \brief Executes conditional <em>DELETE FROM table WHERE expr</em> query with parameter binding

   The method supports variable count of QVariant parameters.
   Parameters are bound with <em>QSqlQuery::addBindValue</em>.
   \code
   //DELETE FROM table WHERE a=1 AND b=2
   t.deleteFrom("table").where("a=? AND b=?", 1, 2);
   \endcode
   */
   NonQueryResult where(const QString &expr, const QVariant &last)
   {
      m_params.append(last);

      const QString &sql = createSql(m_table, expr);

      m_query.prepare(sql);

      for (int i = 0; i < m_params.count(); ++i)
      {
         m_query.addBindValue(m_params.at(i));
      }

      const bool res = m_query.exec();

#ifdef DB_EXCEPTIONS_ENABLED

      if (!res)
         throw DBException(m_query);

#endif

      return NonQueryResult(m_query);
   }

   template <typename... Rest> NonQueryResult where(const QString &expr, const QVariant &first, const Rest&... rest)
   {
      m_params.append(first);
      return where(expr, rest...);
   }

   /*!
   \brief Executes unconditional <em>DELETE FROM table</em> query

   \code
   //DELETE FROM table
   t.deleteFrom("table").exec();
   \endcode
   */
   NonQueryResult exec()
   {
      const QString &sql = createSql(m_table);

      const bool res = m_query.exec(sql);

#ifdef DB_EXCEPTIONS_ENABLED

      if (!res)
         throw DBException(m_query);

#endif

      return NonQueryResult(m_query);
   }


private:
   QSqlQuery m_query;
   QString m_table;
   QVariantList m_params;

   static QString createSql(const QString &table, const QString &expr = "1=1")
   {
      return QString("DELETE FROM %0 WHERE %1").arg(table).arg(expr);
   }

};

#endif // EASYQTSQL_DELETEQUERY_H
