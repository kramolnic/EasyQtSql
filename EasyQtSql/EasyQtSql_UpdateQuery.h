#ifndef EASYQTSQL_UPDATEQUERY_H
#define EASYQTSQL_UPDATEQUERY_H

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
\brief QSqlQuery wrapper for <em>UPDATE ... SET ... WHERE ...</em> query execution.
*/
class  UpdateQuery
{
public:
   UpdateQuery(const QString &table, const QSqlDatabase &db)
     : q(db)
     , m_table(table)
   {   }

   /*!
   \brief Sets table <em>field</em> to <em>value</em>
   \param field Table field name
   \param value Value to set
   \return UpdateQuery reference

   The method returns an UpdateQuery reference so its call can be chained:

   \code
   //UPDATE table SET a=111, b=222
   t.update("table")
         .set("a", 111) //method chaining
         .set("b", 222)
         .exec();
   \endcode
   */
   UpdateQuery &set(const QString &field, const QVariant &value)
   {
      m_updateMap[field] = value;

      return *this;
   }

   /*!
   \brief Sets table fields to values from <em>map</em>
   \param map Values map. Map key is field name, map value is field value.

   The method returns an UpdateQuery reference so its calls can be chained.

   \code
   //UPDATE table SET a=111, b=222
   t.update("table")
         .set(QVariantMap{ {"a", 111}, {"b", 222} })
         .exec();
   \endcode
   */
   UpdateQuery &set(const QVariantMap &map)
   {
      for (auto it = map.begin(); it != map.end(); ++it)
      {
         m_updateMap[it.key()] = it.value();
      }

      return *this;
   }

   /*!
   \brief Executes update query with <em>expr</em> condition.
   \param expr WHERE condition

   \code
   //UPDATE table SET a=111, b=222 WHERE a=1 AND b=2
   t.update("table")
         .set(QVariantMap{ {"a", 111}, {"b", 222} })
         .where("a=1 AND b=2");
   \endcode
   */
   NonQueryResult where(const QString &expr)
   {
      m_whereExpr = expr;
      return exec();
   }

   /*!
   \brief Executes update query with <em>expr</em> condition and <em>last</em> parameter binding.
   \param expr WHERE condition
   \param last Parameter to bind on WHERE expression

   The method supports variable count of QVariant parameters.
   Parameters are bound with <em>QSqlQuery::addBindValue</em>.
   \code
   //UPDATE table SET a=111, b=222 WHERE a=1 AND b=2
   t.update("table")
         .set(QVariantMap{ {"a", 111}, {"b", 222} })
         .where("a=? AND b=?", 1, 2);
   \endcode
   */
   NonQueryResult where(const QString &expr, const QVariant &last)
   {
      m_params.append(last);
      m_whereExpr = expr;
      return exec();
   }

   template <typename... Rest> NonQueryResult where(const QString &expr, const QVariant &first, const Rest&... rest)
   {
      m_params.append(first);
      return where(expr, rest...);
   }

   /*!
   \brief Executes UPDATE query without conditions
   */
   NonQueryResult exec()
   {
      QString sql = "UPDATE " + m_table + " SET ";
      QStringList fieldValuePairs;

      for (auto it = m_updateMap.begin(); it != m_updateMap.end(); ++it)
      {
         fieldValuePairs.append(it.key() + "=?");
      }

      sql += fieldValuePairs.join(",");

      if (!m_whereExpr.isEmpty())
      {
         sql += " WHERE " + m_whereExpr;
      }

      q.prepare(sql);

      for (auto it = m_updateMap.begin(); it != m_updateMap.end(); ++it)
      {
         q.addBindValue(it.value());
      }

      for (auto it = m_params.begin(); it != m_params.end(); ++it)
      {
         q.addBindValue(*it);
      }

      bool res = q.exec();

#ifdef DB_EXCEPTIONS_ENABLED

      if (!res)
         throw DBException(q);

#endif

      return NonQueryResult(q);
   }

private:
   QSqlQuery q;
   QString m_table;
   QVariantMap m_updateMap;
   QVariantList m_params;
   QString m_whereExpr;
};

#endif // EASYQTSQL_UPDATEQUERY_H
