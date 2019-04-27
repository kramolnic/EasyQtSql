#ifndef EASYQTSQL_UTIL_H
#define EASYQTSQL_UTIL_H

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

#include "EasyQtSql_PreparedQuery.h"

#endif

/*!
\brief Utility functions.

\code   
   t.first("SELECT 1", [](const QueryResult &row){
      qDebug() << row.scalar<bool>();
   });

   t.each("SELECT * FROM table1", [](const QueryResult &row){
      qDebug() << row.toMap();
   });

   PreparedQuery query = t.prepare("SELECT a, b, c, d FROM table1 WHERE a = ? AND b = ?");
   
   Util::each(query.exec(1, 2), [](const QueryResult &row){
      qDebug() << row.toMap();
   });
\endcode
*/
class Util
{
public:

	/*!
   \brief Applies function (lambda) f to each row in res.
   \param res QueryResult 
   \param f Function
   */
   template<typename Func>
   static int each(QueryResult &res, Func&& f)
   {
      int rowCount = 0;

      if (res.isActive())
      {
         while (res.next())
         {
            f(res);
            ++rowCount;
         }
      }

      return rowCount;
   }

   /*!
   \brief Applies function (lambda) f to count rows starting from start index
   \param res QueryResult 
   \param start Index of start row
   \param count Rows count
   \param f Function
   */
   template<typename Func>
   static int range(QueryResult &res, int start, int count, Func&& f)
   {
      Q_ASSERT(start >= 0);
      Q_ASSERT(count >= 0);

      int rowCount = 0;

      if (res.isActive())
      {
         //skip items
         for (int i = 0; i < start; ++i)
         {
            if (!res.next())
               return rowCount;
         }

         for (int i = 0; i < count && res.next(); ++i)
         {
            f(res);
            rowCount++;
         }
      }

      return rowCount;
   }

   /*!
   \brief Applies function (lambda) f to topCount rows from res
   \param res QueryResult 
   \param topCount Count of rows
   \param count Rows count
   \param f Function
   */
   template<typename Func>
   static int top(QueryResult &res, int topCount, Func&& f)
   {
      return range(res, 0, topCount, f);
   }
   
   /*!
   \brief Applies function (lambda) f to first result row
   \param res QueryResult    
   \param f Function
   */
   template<typename Func>
   static int first(QueryResult &res, Func&& f)
   {
      return top(res, 1, f);
   }

private:
   Util(){}
};

#endif // EASYQTSQL_UTIL_H
