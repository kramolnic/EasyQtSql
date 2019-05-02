#include <QtTest>
#include <QMetaObject>
#include "EasyQtSql.h"
#include "../Shared/Shared.h"

using namespace EasyQtSql;

class TestSelect : public QObject
{
   Q_OBJECT

private:

   //====================================================
   // Test data

   const QList<Row> &testData() const
   {
      const static QList<Row> rows =
      {  {1, 2, 3, "a"}
       , {4, 5, 6, "b"}
       , {7, 8, 9, "c"}
      };

      return rows;
   }

   //====================================================

public:
   TestSelect(){}
   ~TestSelect(){}

private slots:
   void initTestCase();
   void cleanupTestCase();
   void test_case1();
   void test_case2();
   void test_case3();
   void test_case4();
   void test_case5();
   void test_case6();
   void test_case7();
   void test_case8();
   void test_case9();
   void test_case10();
   void test_case11();
   void test_case12();
   void test_case13();
   void test_case14();
   void test_case14_1();
   void test_case15();
   void test_case16();
   void test_case17();

private:

   int colCount() const
   {
      static Row sampleRow;

      return sampleRow.staticMetaObject.propertyCount();
   }

   int rowCount() const
   {
      return testData().count();
   }

   const char *selectABCDQuery = "SELECT a, b, c, d FROM testTable";
};

void TestSelect::initTestCase()
{
   QLatin1Literal driverName("QSQLITE");

   if (!QSqlDatabase::drivers().contains(driverName))
       QFAIL("This test requires the SQLITE database driver");

   QSqlDatabase sdb = QSqlDatabase::addDatabase(driverName);

   sdb.setDatabaseName(":memory:");

   if (!sdb.open())
   {
      QFAIL(sdb.lastError().text().toStdString().c_str());
   }

   Transaction t(sdb);

   t.execNonQuery("CREATE TABLE testTable (a int, b int, c int, d text)");

   const auto &rows = testData();

   //multi insert
   NonQueryResult res1 = t.insertInto("testTable (a, b, c, d)")
      .values(rows[0].a, rows[0].b, rows[0].c, rows[0].d)
      .values(rows[1].a, rows[1].b, rows[1].c, rows[1].d)
      .values(rows[2].a, rows[2].b, rows[2].c, rows[2].d)
      .exec();

   t.commit();
}

void TestSelect::cleanupTestCase()
{
   {
      QSqlDatabase sdb = QSqlDatabase::database(QSqlDatabase::defaultConnection);
      if (sdb.isOpen())
      {
         sdb.close();
      }
   }

   QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
}

void TestSelect::test_case1() //row count
{
   Transaction t;

   QueryResult res = t.execQuery("SELECT COUNT(*) FROM testTable");

   if (res.next())
   {
      int count = -1;
      res.fetchVars(count);

      QCOMPARE(count, rowCount());
   }
   else
   {
      QFAIL("No query result");
   }
}

void TestSelect::test_case2() //col count
{
   Transaction t;

   QueryResult res = t.execQuery("SELECT * FROM testTable");

   if (res.next())
   {
      QVariantList list;
      res.fetchList(list);

      QCOMPARE(list.count(), colCount());
   }
   else
   {
      QFAIL("No query result");
   }
}

void TestSelect::test_case3() //select specified columns
{
   Transaction t;

   QueryResult res = t.execQuery("SELECT a, c FROM testTable");

   if (res.next())
   {
      QVariantList list;
      res.fetchList(list);

      QCOMPARE(list.count(), 2); //two columns selected (a, c)
   }
   else
   {
      QFAIL("No query result");
   }
}

void TestSelect::test_case4() //select rows with condition
{
   Transaction t;

   QueryResult res = t.execQuery("SELECT COUNT(*) FROM testTable WHERE a = 4 OR b = 8");

   if (res.next())
   {
      int count = -1;
      res.fetchVars(count);

      QCOMPARE(count, 2); //2 rows expected (a = 4 OR b = 8)
   }
   else
   {
      QFAIL("No query result");
   }
}

void TestSelect::test_case5() //select rows with condition (prepare stmt)
{
   Transaction t;

   PreparedQuery query = t.prepare("SELECT COUNT(*) FROM testTable WHERE a = ? OR b = ?");

   QueryResult res = query.exec(4, 8);

   if (res.next())
   {
      int count = -1;
      res.fetchVars(count);

      QCOMPARE(count, 2); //2 rows expected (a = 4 OR b = 8)
   }
   else
   {
      QFAIL("No query result");
   }
}

void TestSelect::test_case6() //fetch rows
{
   Transaction t;

   QueryResult res = t.execQuery(selectABCDQuery);

   const auto &rows = testData();

   int i = 0;

   while (res.next())
   {
      int a, b, c;
      QString d;

      res.fetchVars(a, b, c, d);

      const Row &row = rows[i];

      QCOMPARE(a, row.a);
      QCOMPARE(b, row.b);
      QCOMPARE(c, row.c);
      QCOMPARE(d, row.d);

      i++;
   }

   QCOMPARE(i, rows.count());
}

void TestSelect::test_case7() //fetch rows
{
   Transaction t;

   QueryResult res = t.execQuery(selectABCDQuery);

   const auto &rows = testData();

   int i = 0;

   while (res.next())
   {
      QVariantList list;
      res.fetchList(list);

      const Row &row = rows[i];

      QCOMPARE(list.value(0).toInt()   , row.a);
      QCOMPARE(list.value(1).toInt()   , row.b);
      QCOMPARE(list.value(2).toInt()   , row.c);
      QCOMPARE(list.value(3).toString(), row.d);

      i++;
   }

   QCOMPARE(i, rows.count());
}

void TestSelect::test_case8() //fetch rows
{
   Transaction t;

   QueryResult res = t.execQuery(selectABCDQuery);

   const auto &rows = testData();

   int i = 0;

   while (res.next())
   {
      QVariantMap map;
      res.fetchMap(map);

      const Row &row = rows[i];

      QCOMPARE(map.value("a").toInt()   , row.a);
      QCOMPARE(map.value("b").toInt()   , row.b);
      QCOMPARE(map.value("c").toInt()   , row.c);
      QCOMPARE(map.value("d").toString(), row.d);

      i++;
   }

   QCOMPARE(i, rows.count());
}

void TestSelect::test_case9() //fetch rows
{
   Transaction t;

   QueryResult res = t.execQuery(selectABCDQuery);

   const auto &rows = testData();

   int i = 0;

   while (res.next())
   {
      QStringList list;
      res.fetchStringList(list);

      const Row &row = rows[i];

      QCOMPARE(list.value(0), QString::number(row.a));
      QCOMPARE(list.value(1), QString::number(row.b));
      QCOMPARE(list.value(2), QString::number(row.c));
      QCOMPARE(list.value(3), row.d);

      i++;
   }

   QCOMPARE(i, rows.count());
}

void TestSelect::test_case10() //fetch rows
{
   Transaction t;

   QueryResult res = t.execQuery(selectABCDQuery);

   const auto &rows = testData();

   int i = 0;

   while (res.next())
   {
      QVector<QVariant> vector;
      res.fetchVector(vector);

      const Row &row = rows[i];

      QCOMPARE(vector.value(0).toInt()   , row.a);
      QCOMPARE(vector.value(1).toInt()   , row.b);
      QCOMPARE(vector.value(2).toInt()   , row.c);
      QCOMPARE(vector.value(3).toString(), row.d);

      i++;
   }

   QCOMPARE(i, rows.count());
}

void TestSelect::test_case11() //fetch rows
{
   Transaction t;

   QueryResult res = t.execQuery(selectABCDQuery);

   const auto &rows = testData();

   int i = 0;

   while (res.next())
   {
      TestObject testRow;
      res.fetchObject(testRow);

      const Row &row = rows[i];

      QCOMPARE(testRow.a, row.a);
      QCOMPARE(testRow.b, row.b);
      QCOMPARE(testRow.c, row.c);
      QCOMPARE(testRow.d, row.d);

      i++;
   }

   QCOMPARE(i, rows.count());
}

void TestSelect::test_case12() //fetch rows
{
   Transaction t;

   QueryResult res = t.execQuery(selectABCDQuery);

   const auto &rows = testData();

   int i = 0;

   while (res.next())
   {
      Row testRow;
      res.fetchGadget(testRow);

      const Row &row = rows[i];

      QCOMPARE(testRow.a, row.a);
      QCOMPARE(testRow.b, row.b);
      QCOMPARE(testRow.c, row.c);
      QCOMPARE(testRow.d, row.d);

      i++;
   }

   QCOMPARE(i, rows.count());
}

void TestSelect::test_case13() //fetch rows
{
   Transaction t;

   QueryResult res = t.execQuery(selectABCDQuery);

   const auto &rows = testData();

   int i = 0;

   while (res.next())
   {
      const QVector<int> intVector = res.toVector<int>();

      const Row &row = rows[i];

      const QVector<int> expected = { row.a, row.b, row.c, row.d.toInt() };

      QCOMPARE(intVector, expected);

      i++;
   }

   QCOMPARE(i, rows.count());
}

void TestSelect::test_case14() //fetch rows
{
   Transaction t;

   QueryResult res = t.execQuery(selectABCDQuery);

   const auto &rows = testData();

   int i = 0;

   while (res.next())
   {
      const QVector<int> intVector = res.parseToIntVector();

      const Row &row = rows[i];

      const QVector<int> expected = { row.a, row.b, row.c }; //string value d is not parseable with base 10 in this case (0xA, 0xB, 0xC)

      qDebug() << intVector << expected;

      QCOMPARE(intVector, expected);

      i++;
   }

   QCOMPARE(i, rows.count());
}

void TestSelect::test_case14_1() //fetch rows
{
   Transaction t;

   QueryResult res = t.execQuery(selectABCDQuery);

   const auto &rows = testData();

   int i = 0;

   while (res.next())
   {
      const QVector<int> intVector = res.parseToIntVector(16);

      const Row &row = rows[i];

      const QVector<int> expected = { row.a, row.b, row.c, row.d.toInt(Q_NULLPTR, 16) }; //string value d is parsible with base 16 ("a", "b", "c")

      qDebug() << intVector << expected;

      QCOMPARE(intVector, expected);

      i++;
   }

   QCOMPARE(i, rows.count());
}

void TestSelect::test_case15() //fetch rows
{
   Transaction t;

   QueryResult res = t.execQuery(selectABCDQuery);

   const auto &rows = testData();

   int i = 0;

   while (res.next())
   {
      const QVector<bool> boolVector = res.toVector<bool>();

      const Row &row = rows[i];

      const QVector<bool> expected = { QVariant(row.a).toBool(), QVariant(row.b).toBool(), QVariant(row.c).toBool(), QVariant(row.d).toBool() };

      QCOMPARE(boolVector, expected);

      i++;
   }

   QCOMPARE(i, rows.count());
}

void TestSelect::test_case16() //fetch rows
{
   Transaction t;

   QueryResult res = t.execQuery(selectABCDQuery);

   const auto &rows = testData();

   int i = 0;

   while (res.next())
   {
      const QVector<QString> strVector = res.toVector<QString>();

      const Row &row = rows[i];

      const QVector<QString> expected = { QVariant(row.a).toString(), QVariant(row.b).toString(), QVariant(row.c).toString(), row.d };

      QCOMPARE(strVector, expected);

      i++;
   }

   QCOMPARE(i, rows.count());
}

void TestSelect::test_case17() //fetch rows
{
   Transaction t;

   const auto &rows = testData();

   {
      int curRow = 0;
      QVector<Row> actualRows;

      const int count = t.each(selectABCDQuery, [&curRow, &rows, &actualRows](const QueryResult &row)
      {
         const auto &rowData = rows.at(curRow);

         Row r;
         r.a = row.value(0).toInt();
         r.b = row.value(1).toInt();
         r.c = row.value(2).toInt();
         r.d = row.value(3).toString();

         actualRows.append(r);

         QCOMPARE(r.a, rowData.a);
         QCOMPARE(r.b, rowData.b);
         QCOMPARE(r.c, rowData.c);
         QCOMPARE(r.d, rowData.d);

         ++curRow;
      });

      QCOMPARE(count, rowCount());
      QCOMPARE(actualRows.count(), rowCount());
   }

   {
      const int count = t.first(selectABCDQuery, [&rows](const QueryResult &row)
      {
         const auto &rowData = rows.at(0);

         QCOMPARE(row.value(0).toInt(), rowData.a);
         QCOMPARE(row.value(1).toInt(), rowData.b);
         QCOMPARE(row.value(2).toInt(), rowData.c);
         QCOMPARE(row.value(3).toString(), rowData.d);
      });

      QCOMPARE(count, 1);
   }

   {
      int curRow = 0;
      const int topCount = 2;
      QVector<Row> actualRows;

      const int count = t.top(selectABCDQuery, topCount, [&curRow, &rows, &actualRows](const QueryResult &row)
      {
         const auto &rowData = rows.at(curRow);

         Row r;
         r.a = row.value(0).toInt();
         r.b = row.value(1).toInt();
         r.c = row.value(2).toInt();
         r.d = row.value(3).toString();

         actualRows.append(r);

         QCOMPARE(r.a, rowData.a);
         QCOMPARE(r.b, rowData.b);
         QCOMPARE(r.c, rowData.c);
         QCOMPARE(r.d, rowData.d);

         ++curRow;
      });

      QCOMPARE(count, topCount);
      QCOMPARE(actualRows.count(), topCount);
   }

   {
      int curRow = 1;
      const int resCount = 1;
      QVector<Row> actualRows;

      const int count = t.range(selectABCDQuery, curRow, resCount, [&curRow, &rows, &actualRows](const QueryResult &row)
      {
         const auto &rowData = rows.at(curRow);

         qDebug() << row.toMap() << curRow;

         Row r;
         r.a = row.value(0).toInt();
         r.b = row.value(1).toInt();
         r.c = row.value(2).toInt();
         r.d = row.value(3).toString();

         actualRows.append(r);

         QCOMPARE(r.a, rowData.a);
         QCOMPARE(r.b, rowData.b);
         QCOMPARE(r.c, rowData.c);
         QCOMPARE(r.d, rowData.d);

         ++curRow;
      });

      QCOMPARE(count, resCount);
      QCOMPARE(actualRows.count(), resCount);
   }

   {
      int curRow = 0;
      const int resCount = 3;
      QVector<Row> actualRows;

      const int count = t.range(selectABCDQuery, curRow, resCount, [&curRow, &rows, &actualRows](const QueryResult &row)
      {
         const auto &rowData = rows.at(curRow);

         qDebug() << row.toMap() << curRow;

         Row r;
         r.a = row.value(0).toInt();
         r.b = row.value(1).toInt();
         r.c = row.value(2).toInt();
         r.d = row.value(3).toString();

         actualRows.append(r);

         QCOMPARE(r.a, rowData.a);
         QCOMPARE(r.b, rowData.b);
         QCOMPARE(r.c, rowData.c);
         QCOMPARE(r.d, rowData.d);

         ++curRow;
      });

      QCOMPARE(count, resCount);
      QCOMPARE(actualRows.count(), resCount);
   }

   {
      int curRow = 0;
      const int resCount = 10;
      QVector<Row> actualRows;

      const int count = t.range(selectABCDQuery, curRow, resCount, [&curRow, &rows, &actualRows](const QueryResult &row)
      {
         const auto &rowData = rows.at(curRow);

         qDebug() << row.toMap() << curRow;

         Row r;
         r.a = row.value(0).toInt();
         r.b = row.value(1).toInt();
         r.c = row.value(2).toInt();
         r.d = row.value(3).toString();

         actualRows.append(r);

         QCOMPARE(r.a, rowData.a);
         QCOMPARE(r.b, rowData.b);
         QCOMPARE(r.c, rowData.c);
         QCOMPARE(r.d, rowData.d);

         ++curRow;
      });

      QCOMPARE(count, rowCount());
      QCOMPARE(actualRows.count(), rowCount());
   }

   {
      const int count = t.range(selectABCDQuery, 3, 10, [](const QueryResult &row)
      {
         Q_UNUSED(row);
         QFAIL("UNSEXPECTED");
      });

      QCOMPARE(count, 0);
   }

   {
      const int count = t.range(selectABCDQuery, 10, 10, [](const QueryResult &row)
      {
         Q_UNUSED(row);
         QFAIL("UNSEXPECTED");
      });

      QCOMPARE(count, 0);
   }
}

QTEST_APPLESS_MAIN(TestSelect)

#include "tst_testselect.moc"
