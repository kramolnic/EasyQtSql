#ifndef EASYQTSQL_DBEXCEPTION_H
#define EASYQTSQL_DBEXCEPTION_H

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
 * \brief Exception class for SQL errors handling
 */
class DBException : public QException
{
   friend class Database;
   friend class Transaction;
   friend class PreparedQuery;
   friend class InsertQuery;
   friend class UpdateQuery;
   friend class DeleteQuery;

public:
   const QSqlError lastError;
   const QString   lastQuery;

private:
   explicit DBException (const QSqlQuery &q)
    : lastError(q.lastError())
    , lastQuery(q.lastQuery())
   { }

   explicit DBException (const QSqlDatabase &db)
    : lastError(db.lastError())
   { }
};

#endif // EASYQTSQL_DBEXCEPTION_H
