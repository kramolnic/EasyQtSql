#ifndef EASYQTSQL_NONQUERYRESULT_H
#define EASYQTSQL_NONQUERYRESULT_H

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

#endif

/*!
 * \brief QSqlQuery wrapper for non-select query results reading
 */
class NonQueryResult
{
   friend class Database;
   friend class Transaction;   
   friend class InsertQuery;
   friend class UpdateQuery;
   friend class DeleteQuery;

public:

   /*!
   \brief Returns reference on wrapped QSqlQuery.
   */
   QSqlQuery &unwrappedQuery()
   {
      return m_query;
   }

   /*!
   \brief Returns the number of rows affected by the result's SQL statement, or -1 if it cannot be determined.

   Wrapper over QSqlQuery::numRowsAffected()
   */
   int numRowsAffected() const
   {
      return m_query.numRowsAffected();
   }

   /*!
   \brief Returns the object ID of the most recent inserted row if the database supports it.

   Wrapper over QSqlQuery::lastInsertId()
   */
   QVariant lastInsertId() const
   {
      return m_query.lastInsertId();
   }

   /*!
   \brief Returns error information about the last error (if any) that occurred with this query.

   Wrapper over QSqlQuery::lastError()
   */
   QSqlError lastError() const
   {
      return m_query.lastError();
   }

   /*!
   \brief Returns the text of the current query being used, or an empty string if there is no current query text.

   Wrapper over QSqlQuery::lastQuery()
   */
   QString lastQuery() const
   {
      return m_query.lastQuery();
   }

   /*!
   \brief Returns the last query that was successfully executed.

   Wrapper over QSqlQuery::executedQuery()
   */
   QString executedQuery() const
   {
      return m_query.executedQuery();
   }

private:
   NonQueryResult(const QSqlQuery &q)
    : m_query(q)
   { }

   QSqlQuery m_query;
};

#endif // EASYQTSQL_NONQUERYRESULT_H
