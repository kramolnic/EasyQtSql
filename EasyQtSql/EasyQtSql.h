#ifndef EASYQTSQL_H
#define EASYQTSQL_H

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

/*!
\file
\brief Main EasyQtSql header file

To use all the EasyQtSql features just include this header file into your project
\warning Please use EasyQtSql.h file only! You should never include any of EasyQtSql_*.h files.
*/

#include <QtSql>

/*!
   \brief Easy SQL data access helper for QtSql
   \author Alexey <kramolnic> Kramin
   \version 1.0
   \date November 2018
   \verbinclude License.txt

*/

namespace EasyQtSql
{

/*!
  \brief Undefine to disable EasyQtSql exceptions (DBException) globally
 */
#define DB_EXCEPTIONS_ENABLED
//#undef DB_EXCEPTIONS_ENABLED

/// \cond
#define EASY_QT_SQL_MAIN
/// \endcond

//Generic classes
#include "EasyQtSql_DBException.h"
#include "EasyQtSql_ParamDirectionWrapper.h"

//Select query and query results
#include "EasyQtSql_NonQueryResult.h"
#include "EasyQtSql_QueryResult.h"
#include "EasyQtSql_PreparedQuery.h"

//Insert/Update/Delete (CUD) operations
#include "EasyQtSql_InsertQuery.h"
#include "EasyQtSql_UpdateQuery.h"
#include "EasyQtSql_DeleteQuery.h"

//Transaction helper
#include "EasyQtSql_Transaction.h"

#undef EASY_QT_SQL_MAIN

}

#endif // EASYQTSQL_H
