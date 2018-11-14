#ifndef EASYQTSQL_INSERTQUERY_H
#define EASYQTSQL_INSERTQUERY_H

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
\brief QSqlQuery wrapper for <em>INSERT INTO table</em> query execution.
*/
class  InsertQuery
{
public:
   InsertQuery(const QString &table, const QSqlDatabase &db)
     : m_table(table)
     , q(db)
   {   }

   /*!
   \brief Adds list of insert-values to INSERT INTO table(...) VALUES ... query

   The method supports variable count of QVariant parameters.
   Parameters are bound with <em>QSqlQuery::addBindValue</em>.
   \code
   NonQueryResult res = t.insertInto("table (a, b, c, d)")
         .values(1, 2, 3, "a")
         .values(4, 5, 6, "b")
         .values(7, 8, 9, "c")
         .exec();
   \endcode
   */
   InsertQuery &values(const QVariant& t)
   {
      m_args.append(t);

      if (m_insertArray.isEmpty())
      {
         m_insertArray.resize(m_args.count());
      }
      else
      {
         Q_ASSERT(m_insertArray.size() == m_args.size());
      }

      for (int i = 0; i < m_args.count(); ++i)
      {
         m_insertArray[i].append(m_args.at(i));
      }

      m_args.clear();

      return *this;
   }

   template <typename... Rest> InsertQuery &values(const QVariant &first, const Rest&... rest)
   {
      m_args.append(first);

      return values(rest...);
   }

   /*!
   \brief Executes prepared InsertQuery with insert values list
   */
   NonQueryResult exec()
   {
      QString sql = QLatin1String("INSERT INTO ");
      sql.append(m_table);
      sql.append(QLatin1String(" VALUES ("));

      QStringList lst;
      for (int i = 0; i < m_insertArray.count(); ++i)
      {
         lst.append(QLatin1String("?"));
      }

      sql.append(lst.join(QLatin1String(",")));
      sql.append(QLatin1String(")"));

      q.prepare(sql);

      bool res = false;

      if (m_insertArray.count() > 0 && m_insertArray[0].count() > 1)
      {
         for (int i = 0; i < m_insertArray.count(); ++i)
         {
            q.addBindValue(m_insertArray.at(i));
         }

         res = q.execBatch();
      }
      else
      {
         for (int i = 0; i < m_insertArray.count(); ++i)
         {
            q.addBindValue(m_insertArray.at(i).first());
         }

         res = q.exec();
      }

      m_args.clear();
      m_insertArray.clear();

#ifdef DB_EXCEPTIONS_ENABLED

      if (!res)
         throw DBException(q);

#endif

      return NonQueryResult(q);
   }

private:
   QString m_table;
   QSqlQuery q;
   QVariantList m_args;
   QVector<QVariantList> m_insertArray;
};

#endif // EASYQTSQL_INSERTQUERY_H
