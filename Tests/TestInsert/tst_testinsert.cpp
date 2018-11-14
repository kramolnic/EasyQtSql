#include <QtTest>
#include "EasyQtSql.h"
#include "../Shared/Shared.h"

using namespace  EasyQtSql;

class TestInsert : public QObject
{
   Q_OBJECT

public:
   TestInsert(){}
   ~TestInsert(){}

private slots:
   void initTestCase();
   void cleanupTestCase();
   void test_case1();
   void test_case2();
   void test_case3();

};

void TestInsert::initTestCase()
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

   try {

      Transaction t(sdb);

      t.execNonQuery("CREATE TABLE testTable (a int, b int, c int, d text)");

      t.commit();

   }
   catch (const DBException &ex)
   {
      QFAIL(ex.lastError.text().toStdString().c_str());
   }
}

void TestInsert::cleanupTestCase()
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

void TestInsert::test_case1()
{
   const QVector<Row> rows = { {1, 2, 3, "a"}, {4, 5, 6, "b"}, {7, 8, 9, "c"}, {10, 11, 12, "d"}};

   try
   {
      Transaction t;

      InsertQuery query = t.insertInto("testTable (a, b, c, d)");

      query.values(rows[0].a, rows[0].b, rows[0].c, rows[0].d).exec();
      query.values(rows[1].a, rows[1].b, rows[1].c, rows[1].d).exec();
      query.values(rows[2].a, rows[2].b, rows[2].c, rows[2].d).exec();
      query.values(rows[3].a, rows[3].b, rows[3].c, rows[3].d).exec();

      QueryResult res = t.execQuery("SELECT COUNT(*) FROM testTable");

      res.next();

      int count = -1;
      res.fetchVars(count);

      QCOMPARE(count, 4);

      res = t.execQuery("SELECT a, b, c, d FROM testTable");

      int i = 0;
      while (res.next())
      {
         int a, b, c;
         QString d;

         res.fetchVars(a, b, c, d);

         QCOMPARE(a, rows[i].a);
         QCOMPARE(b, rows[i].b);
         QCOMPARE(c, rows[i].c);
         QCOMPARE(d, rows[i].d);

         ++i;
      }

      Database sdb;

      QueryResult res1 = sdb.execQuery("SELECT a, b, c, d FROM testTable");

      QCOMPARE(res1.next(), true);

      //transaction rolled back
   }
   catch (const DBException &ex)
   {
      QFAIL(ex.lastError.text().toStdString().c_str());
   }

   Database sdb;

   QueryResult res = sdb.execQuery("SELECT a, b, c, d FROM testTable");

   QCOMPARE(res.next(), false);
}

void TestInsert::test_case2()
{
   const QVector<Row> rows = { {1, 2, 3, "a"}, {4, 5, 6, "b"}, {7, 8, 9, "c"}, {10, 11, 12, "d"}};

   try
   {
      Transaction t;

      InsertQuery query = t.insertInto("testTable (a, b, c, d)");

      query.values(rows[0].a, rows[0].b, rows[0].c, rows[0].d)
           .values(rows[1].a, rows[1].b, rows[1].c, rows[1].d)
           .values(rows[2].a, rows[2].b, rows[2].c, rows[2].d)
           .values(rows[3].a, rows[3].b, rows[3].c, rows[3].d).exec();

      QueryResult res = t.execQuery("SELECT COUNT(*) FROM testTable");

      res.next();

      int count = -1;
      res.fetchVars(count);

      QCOMPARE(count, 4);

      res = t.execQuery("SELECT a, b, c, d FROM testTable");

      int i = 0;
      while (res.next())
      {
         int a, b, c;
         QString d;

         res.fetchVars(a, b, c, d);

         QCOMPARE(a, rows[i].a);
         QCOMPARE(b, rows[i].b);
         QCOMPARE(c, rows[i].c);
         QCOMPARE(d, rows[i].d);

         ++i;
      }

      Database sdb;

      QueryResult res1 = sdb.execQuery("SELECT a, b, c, d FROM testTable");

      QCOMPARE(res1.next(), true);

      //transaction rolled back
   }
   catch (const DBException &ex)
   {
      QFAIL(ex.lastError.text().toStdString().c_str());
   }

   Database sdb;

   QueryResult res = sdb.execQuery("SELECT a, b, c, d FROM testTable");

   QCOMPARE(res.next(), false);
}

void TestInsert::test_case3()
{
   const QVector<Row> rows = { {1, 2, 3, "a"}, {4, 5, 6, "b"}, {7, 8, 9, "c"}, {10, 11, 12, "d"}};

   QVERIFY_EXCEPTION_THROWN({
      Transaction t;

      InsertQuery query = t.insertInto("testTable (a, b, c, d)");

      //parameters count mismatch
      query.values(rows[0].a, rows[0].b, rows[0].c).exec();
      query.values(rows[1].a, rows[1].b, rows[1].c).exec();
      query.values(rows[2].a, rows[2].b, rows[2].c).exec();
      query.values(rows[3].a, rows[3].b, rows[3].c).exec();

      //transaction rolled back
   }, DBException);
}


QTEST_APPLESS_MAIN(TestInsert)

#include "tst_testinsert.moc"
