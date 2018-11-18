#ifndef EASYQTSQL_QUERYRESULT_H
#define EASYQTSQL_QUERYRESULT_H

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
\brief QSqlQuery wrapper with handy data fetch methods.
*/
class  QueryResult
{
   friend class Database;
   friend class Transaction;
   friend class PreparedQuery;   

public:

   /*!
   \brief Returns reference on wrapped QSqlQuery.
   */
   QSqlQuery &unwrappedQuery()
   {
      return m_query;
   }

   /*!
   \brief Retrieves the next record in the result, if available, and positions the query on the retrieved record.

   Wrapper over QSqlQuery::next()
   */
   bool next()
   {
      bool res = m_query.next();

      if ( ! m_firstRowFetched)
      {
         m_fieldNames.clear();

         QSqlRecord record = m_query.record();

         for (int i = 0; i < record.count(); ++i)
         {
            m_fieldNames.append(record.fieldName(i));
         }

         m_firstRowFetched = true;
      }

      return res;
   }

   /*!
   \brief Retrieves the previous record in the result, if available, and positions the query on the retrieved record.

   Wrapper over QSqlQuery::previous()
   */
   bool previous()
   {
      return m_query.previous();
   }

   /*!
   \brief Discards the current result set and navigates to the next if available (if database is capable of returning multiple result sets)

   Wrapper over QSqlQuery::nextResult()
   */
   bool nextResult()
   {
      m_firstRowFetched = false;

      bool res = m_query.nextResult();

      return res;
   }

   /*!
   \brief Retrieves the first record in the result, if available, and positions the query on the retrieved record.

   Wrapper over QSqlQuery::first()
   */
   bool first()
   {
      return m_query.first();
   }

   /*!
   \brief Retrieves the last record in the result, if available, and positions the query on the retrieved record.

   Wrapper over QSqlQuery::last()
   */
   bool last()
   {
      return m_query.last();
   }

   /*!
   \brief Retrieves the record at position index, if available, and positions the query on the retrieved record.

   Wrapper over QSqlQuery::seek()
   */
   bool seek(int index, bool relative = false)
   {
      return m_query.seek(index, relative);
   }

   /*!
   \brief Returns the current internal position of the query. The first record is at position zero.
   \return If the position is invalid, the function returns QSql::BeforeFirstRow or QSql::AfterLastRow, which are special negative values.

   Wrapper over QSqlQuery::at()
   */
   int at() const
   {
      return m_query.at();
   }

   /*!
   \brief Returns true if the query is active. An active QSqlQuery is one that has been exec()'d successfully but not yet finished with.

   Wrapper over QSqlQuery::isActive()
   */
   bool isActive() const
   {
      return m_query.isActive();
   }

   /*!
   \brief Returns true if the query is currently positioned on a valid record; otherwise returns false.

   Wrapper over QSqlQuery::isValid()
   */
   bool isValid() const
   {
      return m_query.isValid();
   }

   /*!
   \brief Returns true if you can only scroll forward through a result set; otherwise returns false.

   Wrapper over QSqlQuery::isForwardOnly()
   */
   bool isForwardOnly() const
   {
      return m_query.isForwardOnly();
   }

   /*!
   \brief Returns the size of the result (number of rows returned), or -1 if the size cannot be determined or if the database does not support reporting information about query sizes.

   Wrapper over QSqlQuery::size()
   */
   int size() const
   {
      return m_query.size();
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

   /*!
   \brief Returns the value of field index in the current record.

   Wrapper over QSqlQuery::value()
   */
   QVariant value(int column) const
   {
      return m_query.value(column);
   }

   /*!
   \brief Returns the value of the field called name in the current record. If field name does not exist an invalid variant is returned.

   Wrapper over QSqlQuery::value()
   */
   QVariant value(const QString &colName) const
   {
      return m_query.value(colName);
   }

   /*!
   \brief Returns the value for the placeholder at position column.

   Wrapper over QSqlQuery::boundValue()
   */
   QVariant boundValue(int column) const
   {
      return m_query.boundValue(column);
   }

   /*!
   \brief Returns the value for the placeholder with aliasName.

   Use subclasses of ParamDirectionWrapper class (::In, ::Out, ::InOut) for aliased/directional parameter binding during PreparedQuery::exec execution

   Use Out and InOut wrappers if you need to read Stored Procedures return values (output parameters).

   \sa ::In, ::Out, ::InOut, PreparedQuery::exec
   */
   QVariant boundValue(const QString &aliasName) const
   {
      const int index = m_bindValueAlias.value(aliasName.trimmed().toLower(), -1);

      QVariant res;

      if (index >= 0)
      {
         res = boundValue(index);
      }

      return res;
   }

   /*!
   \brief Returns list of the bound values (with positional binding)

   Wrapper over QSqlQuery::boundValues()
   */
   QVariantList boundValues() const
   {
      QList<QVariant> list = m_query.boundValues().values();

      return list;
   }

   /*!
   \brief Returns QVariantMap filled with values fetched from current result row. Key is QString (result column name) and value is QVariant value.
   \sa QueryResult::fetchMap
   */
   QVariantMap toMap() const
   {
      QVariantMap res;

      fetchMap(res);

      return res;
   }

   /*!
   \brief Returns QVariantList filled with values fetched from current result row.
   \sa QueryResult::fetchList
   */
   QVariantList toList() const
   {
      QVariantList res;

      fetchList(res);

      return res;
   }

   /*!
   \brief Returns QStringList filled with values fetched from current result row. SQL values converted to QString with QVariant::toString() method.
   \sa QueryResult::fetchStringList()
   */
   QStringList toStringList() const
   {
      QStringList res;

      fetchStringList(res);

      return res;
   }

   /*!
   \brief Returns QVector<QVariant> filled with values fetched from current result row. SQL values not converted: QSqlQuery::value() return value used.
   \sa QueryResult::fetchVector
   */
   QVector<QVariant> toVector() const
   {
      QVector<QVariant> res;

      fetchVector(res);

      return res;
   }

   /*!
   \brief Returns QVector<T> filled with values fetched from current result row. SQL values converted with QVariant::value<T>().
   QVector::canConvert<T>() method used for convertability check. Non-convertable values skipped.
   \param skipNullValues Skip SQL NULL values. Resulting value is default value if skipNullValues = false.
   \sa QueryResult::toVector
   */
   template <typename T>
   QVector<T> toVector(bool skipNullValues = false) const
   {
      QVector<T> res;

      res.reserve(m_fieldNames.count());

      for (int i = 0; i < m_fieldNames.count(); ++i)
      {
         const QVariant &value = m_query.value(i);

         if (!value.canConvert<T>())
            continue;

         if (skipNullValues && value.isNull())
            continue;

         res.append(value.value<T>());
      }

      return res;
   }

   /*!
   \brief Returns vector of (optionally parsed) integer values from currently active result row.

   Parsing with specified base will be performed if type of selected database field is String (varchar/text).
   Not successfully parsed string values are ignored.

   \param base Number base to parse int values from SQL string values.
   \param skipNullValues Skip SQL NULL values. Resulting int value is 0 if skipNullValues = false.
   */
   QVector<int> parseToIntVector(int base = 10, bool skipNullValues = false) const
   {
      QVector<int> res;

      res.reserve(m_fieldNames.count());

      for (int i = 0; i < m_fieldNames.count(); ++i)
      {
         const QVariant &value = m_query.value(i);

         if (!value.canConvert<int>())
            continue;

         if (skipNullValues && value.isNull())
            continue;

         bool ok = false;
         int iValue = 0;

         if ( ((value.type() == QVariant::String) || (value.type() == QVariant::ByteArray)))
         {
            const QString str = value.toString();

            iValue = str.toInt(&ok, base);
         }
         else
         {
            iValue = value.toInt(&ok);
         }

         if (ok)
         {
            res.append(iValue);
         }
      }

      return res;
   }

   /*!
   \brief Returns scalar value converted to type T (the value of the first column of the current row)
    */
   template <typename T>
   T scalar() const
   {
      return m_query.value(0).value<T>();
   }

   /*!
   \brief Returns scalar value (the value of the first column of the current row)
    */
   QVariant scalar() const
   {
      return m_query.value(0);
   }

   /*!
   \brief Assigns a variable the value from the current record

   The method has a variable number of parameters, which allows filling the list of variables with values from the current record in single call.

   Overloaded <em>EasyQtSql::fetchVars</em> methods performs the corresponding type conversion with QVariant::to*() methods.

   Supported parameter types: <em>int</em>, <em>double</em>, <em>bool</em>, <em>QString</em>, <em>QDate</em>, <em>QDateTime</em>, <em>QTime</em>, <em>QByteArray</em>, <em>QVariant</em>.

   Variables of different types can be mixed.

   \code
   Transaction t;
   QueryResult res = t.execQuery("SELECT a, b, c, d FROM table");

   while (res.next())
   {
      int a;
      bool b;
      QString c;
      QDateTime d;

      res.fetchVars(a, b, c, d);
   }
   \endcode

   \sa  QueryResult::fetchVars(int&) const
      , QueryResult::fetchVars(double&) const
      , QueryResult::fetchVars(QString&) const
      , QueryResult::fetchVars(bool&) const
      , QueryResult::fetchVars(QDate&) const
      , QueryResult::fetchVars(QDateTime&) const
      , QueryResult::fetchVars(QTime&) const
      , QueryResult::fetchVars(QByteArray&) const
      , QueryResult::fetchVars(QVariant&) const
   */
   void fetchVars(int &value) const
   {
      value = m_query.value(m_fetchIndex).toInt();

      m_fetchIndex = 0;
   }

   /*!
   \brief Assigns a variable the value from the current record

   \sa  QueryResult::fetchVars(int&) const
   */
   void fetchVars(double &value) const
   {
      value = m_query.value(m_fetchIndex).toInt();

      m_fetchIndex = 0;
   }

   /*!
   \brief Assigns a variable the value from the current record

   \sa  QueryResult::fetchVars(int&) const
   */
   void fetchVars(QString &value) const
   {
      value = m_query.value(m_fetchIndex).toString();

      m_fetchIndex = 0;
   }

   /*!
   \brief Assigns a variable the value from the current record

   \sa  QueryResult::fetchVars(int&) const
   */
   void fetchVars(bool &value) const
   {
      value = m_query.value(m_fetchIndex).toBool();

      m_fetchIndex = 0;
   }

   /*!
   \brief Assigns a variable the value from the current record

   \sa  QueryResult::fetchVars(int&) const
   */
   void fetchVars(QDate &value) const
   {
      value = m_query.value(m_fetchIndex).toDate();

      m_fetchIndex = 0;
   }

   /*!
   \brief Assigns a variable the value from the current record

   \sa  QueryResult::fetchVars(int&) const
   */
   void fetchVars(QDateTime &value) const
   {
      value = m_query.value(m_fetchIndex).toDateTime();

      m_fetchIndex = 0;
   }

   /*!
   \brief Assigns a variable the value from the current record

   \sa  QueryResult::fetchVars(int&) const
   */
   void fetchVars(QTime &value) const
   {
      value = m_query.value(m_fetchIndex).toTime();

      m_fetchIndex = 0;
   }

   /*!
   \brief Assigns a variable the value from the current record

   \sa  QueryResult::fetchVars(int&) const
   */
   void fetchVars(QByteArray &value) const
   {
      value = m_query.value(m_fetchIndex).toByteArray();

      m_fetchIndex = 0;
   }

   /*!
   \brief Assigns a variable the value from the current record

   \sa  QueryResult::fetchVars(int&) const
   */
   void fetchVars(QVariant &value) const
   {
      value = m_query.value(m_fetchIndex);

      m_fetchIndex = 0;
   }

   template <typename... Rest> void fetchVars(int &value, Rest&... rest) const
   {
      value = m_query.value(m_fetchIndex++).toInt();

      fetchVars(rest...);
   }

   template <typename... Rest> void fetchVars(double &value, Rest&... rest) const
   {
      value = m_query.value(m_fetchIndex++).toDouble();

      fetchVars(rest...);
   }

   template <typename... Rest> void fetchVars(QString &value, Rest&... rest) const
   {
      value = m_query.value(m_fetchIndex++).toString();

      fetchVars(rest...);
   }

   template <typename... Rest> void fetchVars(bool &value, Rest&... rest) const
   {
      value = m_query.value(m_fetchIndex++).toBool();

      fetchVars(rest...);
   }

   template <typename... Rest> void fetchVars(QDate &value, Rest&... rest) const
   {
      value = m_query.value(m_fetchIndex++).toDate();

      fetchVars(rest...);
   }

   template <typename... Rest> void fetchVars(QDateTime &value, Rest&... rest) const
   {
      value = m_query.value(m_fetchIndex++).toDateTime();

      fetchVars(rest...);
   }

   template <typename... Rest> void fetchVars(QTime &value, Rest&... rest) const
   {
      value = m_query.value(m_fetchIndex++).toTime();

      fetchVars(rest...);
   }

   template <typename... Rest> void fetchVars(QByteArray &value, Rest&... rest) const
   {
      value = m_query.value(m_fetchIndex++).toByteArray();

      fetchVars(rest...);
   }

   template <typename... Rest> void fetchVars(QVariant &value, Rest&... rest) const
   {
      value = m_query.value(m_fetchIndex++);

      fetchVars(rest...);
   }

   /*!
   \brief Fills Q_OBJECT object properties with data fetched from current result row.

   Qt Meta-Object System used. Only writable properties can be filled.

   C++ class fields (properties) will be mapped on SQL result table by Q_PROPERTY member names. For example, if Q_OBJECT class contains Q_PROPERTY named "e" but SELECT query defines no "e" column, object property "e" not affected by the method.

   \code

   class TestObject : public QObject
   {
      Q_OBJECT

      //property "a" will be filled with "a" column of the Select SQL query result
      Q_PROPERTY(int a MEMBER a)
      Q_PROPERTY(int b MEMBER b)
      Q_PROPERTY(int c MEMBER c)
      Q_PROPERTY(QString d MEMBER d)

      //property e will be ignored in the example because there is no e field in the Select SQL query result
      Q_PROPERTY(int e MEMBER e)

   private:
      int a;
      int b;
      int c;
      QString d;
      int e;
   };

   void test()
   {
      Transaction t;
      QueryResult res = t.execQuery("SELECT a, b, c, d FROM table");

      while (res.next())
      {
         //TestObject instance (Q_OBJECT class)
         TestObject testRow;
         res.fetchObject(testRow);
      }
   }

   \endcode

   \param metaobject staticMetaObject field from Q_GADGET object
   \param gadget Q_GADGET object pointer.
   */
   void fetchObject(QObject &object) const
   {
      const QMetaObject *metaobject = object.metaObject();
      const int count = metaobject->propertyCount();

      const QVariantMap map = toMap();

      for (int i = 0; i < count; ++i)
      {
         QMetaProperty metaproperty = metaobject->property(i);

         if (metaproperty.isWritable())
         {
            QLatin1String sName(metaproperty.name());

            if (map.contains(sName))
            {
               object.setProperty(sName.data(), map.value(sName));
            }
         }
      }
   }

   /*!
   \brief Fills Q_GADGET object properties with data fetched from current result row.

   Qt Meta-Object System used. Only writable properties can be filled.

   C++ struct fields will be mapped on SQL result table by Q_PROPERTY member names. For example, if Q_GADGET struct contains Q_PROPERTY named "e" but SELECT query defines no "e" column, object property "e" not affected by the method.

   \code
   struct Row
   {
      int a;
      int b;
      int c;
      QString d;
      int e;

   private:
      Q_GADGET

      //property "a" will be filled with "a" column of the Select SQL query result
      Q_PROPERTY(int a MEMBER a)
      Q_PROPERTY(int b MEMBER b)
      Q_PROPERTY(int c MEMBER c)
      Q_PROPERTY(QString d MEMBER d)

      //property e will be ignored in the example because there is no e field in the Select SQL query result
      Q_PROPERTY(int e MEMBER e)
   };

   void test()
   {
      Transaction t;
      QueryResult res = t.execQuery("SELECT a, b, c, d FROM table");

      while (res.next())
      {
         //Row instance (Q_GADGET structure)
         Row testRow;

         //fetch testRow from current result row, properties a, b, c, d will be filled here:
         res.fetchGadget(testRow.staticMetaObject, &testRow);
      }
   }

   \endcode

   \param metaobject staticMetaObject field from Q_GADGET object
   \param gadget Q_GADGET object pointer.
   */
   void fetchGadget(const QMetaObject &metaobject, void *gadget)
   {
      Q_ASSERT(gadget != nullptr);

      const int count = metaobject.propertyCount();

      const QVariantMap map = toMap();

      for (int i = 0; i < count; ++i)
      {
         QMetaProperty metaproperty = metaobject.property(i);

         if (metaproperty.isWritable())
         {
            QLatin1String sName(metaproperty.name());

            if (map.contains(sName))
            {
               metaproperty.writeOnGadget(gadget, map.value(sName));
            }
         }
      }
   }

   /*!
   \brief Fills QVariantMap with values fetched from current result row. Key is QString (result column name) and value is QVariant value.
   \param[out] map QVariantMap
   \sa QueryResult::toMap
   */
   void fetchMap(QVariantMap &map) const
   {
      map.clear();

      for (int i = 0; i < m_fieldNames.count(); ++i)
      {
         map.insert(m_fieldNames.at(i), m_query.value(i));
      }
   }

   /*!
   \brief Fills QVariantList with values fetched from current result row. SQL values not converted: QSqlQuery::value() return value used.
   \param[out] list QVariantList
   \sa QueryResult::toList
   */
   void fetchList(QVariantList &list) const
   {
      list.clear();

      for (int i = 0; i < m_fieldNames.count(); ++i)
      {
         list.append(m_query.value(i));
      }
   }

   /*!
   \brief Fills QVector<QVariant> with values fetched from current result row. SQL values not converted: QSqlQuery::value() return value used.
   \param[out] vector QVector<QVariant>
   \sa QueryResult::toVector
   */
   void fetchVector(QVector<QVariant> &vector) const
   {
      vector.clear();
      vector.reserve(m_fieldNames.count());

      for (int i = 0; i < m_fieldNames.count(); ++i)
      {
         vector.append(m_query.value(i));
      }
   }

   /*!
   \brief Fills QStringList with values fetched from current result row. SQL values converted to QString with QVariant::toString() method.
   \param[out] list QStringList
   \sa QueryResult::toStringList
   */
   void fetchStringList(QStringList &list) const
   {
      list.clear();

      for (int i = 0; i < m_fieldNames.count(); ++i)
      {
         list.append(m_query.value(i).toString());
      }
   }

private:

   QueryResult()
   { }

   QueryResult(const QSqlQuery &query)
    : m_query(query)
   { }

   QueryResult(const QSqlQuery &query, const QMap<QString, int> &bindValueAliasMap)
    : m_query(query)
    , m_bindValueAlias(bindValueAliasMap)
   { }

private:
   QSqlQuery   m_query;
   QStringList m_fieldNames;
   QMap<QString, int> m_bindValueAlias;
   mutable int m_fetchIndex = 0;
   bool m_firstRowFetched = false;
};

#endif // EASYQTSQL_QUERYRESULT_H
