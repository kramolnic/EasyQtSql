#ifndef EASYQTSQL_PREPAREDQUERY_H
#define EASYQTSQL_PREPAREDQUERY_H

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
#include "EasyQtSql_QueryResult.h"
#include "EasyQtSql_ParamDirectionWrapper.h"

#endif

/*!
 \brief Class for query preparation and execution

 \code

   //SELECT a, b, c, d FROM table WHERE a=? AND b=?
      PreparedQuery query = t.prepare("SELECT a, b, c, d FROM table WHERE a=? AND b=?");

   //parameters a and b are bound with values 1 and 2, query executed
      QueryResult res = query.exec(1, 2);

      while(res.next())
      {
         //fetch data here
      }

   //parameters a and b are bound with values 3 and 4, query executed
      res = query.exec(3, 4);

      while(res.next())
      {
         //fetch data here
      }

 \endcode
 */
class  PreparedQuery
{
public:
   PreparedQuery(const QString &stmt, const QSqlDatabase &db, bool forwardOnly = true)
      :m_query(db)
   {
      m_query.setForwardOnly(forwardOnly);
      m_query.prepare(stmt);
   }

   QueryResult &exec()
   {
      m_index = 0;

      const bool res = m_query.exec();

#ifdef DB_EXCEPTIONS_ENABLED

      if (!res)
         throw DBException(m_query);

#endif

      m_result = QueryResult(m_query, m_aliases);

      m_aliases.clear();

      return m_result;
   }

   QueryResult &exec(const QVariant &bindValue)
   {
      m_query.bindValue(m_index, bindValue);

      return exec();
   }

   QueryResult &exec(const In &paramWrapper)
   {
      m_query.bindValue(m_index, paramWrapper.value, QSql::In);

      addAliasIfSet(paramWrapper, m_index);

      return exec();
   }

   QueryResult &exec(const Out &paramWrapper)
   {
      m_query.bindValue(m_index, paramWrapper.value, QSql::Out);

      addAliasIfSet(paramWrapper, m_index);

      return exec();
   }

   QueryResult &exec(const InOut &paramWrapper)
   {
      m_query.bindValue(m_index, paramWrapper.value, QSql::InOut);

      addAliasIfSet(paramWrapper, m_index);

      return exec();
   }

   template <typename... Rest> QueryResult &exec(const QVariant &bindValue, const Rest&... rest)
   {
      m_query.bindValue(m_index, bindValue);

      m_index++;

      return exec(rest...);
   }

   template <typename... Rest> QueryResult &exec(const In &paramWrapper, const Rest&... rest)
   {
      m_query.bindValue(m_index, paramWrapper.value, QSql::In);

      addAliasIfSet(paramWrapper, m_index);

      m_index++;

      return exec(rest...);
   }

   template <typename... Rest> QueryResult &exec(const Out &paramWrapper, const Rest&... rest)
   {
      m_query.bindValue(m_index, paramWrapper.value, QSql::Out);

      addAliasIfSet(paramWrapper, m_index);

      m_index++;

      return exec(rest...);
   }

   template <typename... Rest> QueryResult &exec(const InOut &paramWrapper, const Rest&... rest)
   {
      m_query.bindValue(m_index, paramWrapper.value, QSql::InOut);

      addAliasIfSet(paramWrapper, m_index);

      m_index++;

      return exec(rest...);
   }

private:
   QSqlQuery m_query;

   int m_index = 0;

   QueryResult m_result;

   QMap<QString, int> m_aliases;

   void addAliasIfSet(const ParamDirectionWrapper &paramWrapper, int index)
   {
      if (!paramWrapper.alias.isEmpty())
      {
         m_aliases[paramWrapper.alias] = index;
      }
   }
};

#endif // EASYQTSQL_PREPAREDQUERY_H
