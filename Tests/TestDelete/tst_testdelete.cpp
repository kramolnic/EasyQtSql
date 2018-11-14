#include <QtTest>
#include "EasyQtSql.h"
#include "../Shared/Shared.h"

using namespace EasyQtSql;

class TestDelete : public QObject
{
   Q_OBJECT

public:
   TestDelete(){}
   ~TestDelete(){}

private slots:
   void initTestCase();
   void cleanupTestCase();
   void test_case1();
   void test_case2();
   void test_case3();
   void test_case4();

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

   int colCount() const
   {
      static Row sampleRow;

      return sampleRow.staticMetaObject.propertyCount();
   }

   int rowCount() const
   {
      return testData().count();
   }

};

void TestDelete::initTestCase()
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

void TestDelete::cleanupTestCase()
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

void TestDelete::test_case1()
{
   try
   {
      Transaction t;

      {
         QueryResult res = t.execQuery("SELECT COUNT (*) FROM testTable");

         if (res.next())
         {
            int count = -1;
            res.fetchVars(count);

            QCOMPARE(count, rowCount());
         }
         else
         {
            QFAIL(res.lastError().text().toStdString().c_str());
         }
      }

      t.deleteFrom("testTable").exec();

      {
         QueryResult res = t.execQuery("SELECT COUNT (*) FROM testTable");

         if (res.next())
         {
            int count = -1;
            res.fetchVars(count);

            QCOMPARE(count, 0);
         }
         else
         {
            QFAIL(res.lastError().text().toStdString().c_str());
         }
      }
   }
   catch (const DBException &e)
   {
      QFAIL(e.lastError.text().toStdString().c_str());
   }
}

void TestDelete::test_case2()
{
   try
   {
      Transaction t;

      {
         QueryResult res = t.execQuery("SELECT COUNT (*) FROM testTable");

         if (res.next())
         {
            int count = -1;
            res.fetchVars(count);

            QCOMPARE(count, rowCount());
         }
         else
         {
            QFAIL(res.lastError().text().toStdString().c_str());
         }
      }

      t.deleteFrom("testTable").where("a=1");

      {
         QueryResult res = t.execQuery("SELECT COUNT (*) FROM testTable");

         if (res.next())
         {
            int count = -1;
            res.fetchVars(count);

            QCOMPARE(count, rowCount() - 1);
         }
         else
         {
            QFAIL(res.lastError().text().toStdString().c_str());
         }
      }
   }
   catch (const DBException &e)
   {
      QFAIL(e.lastError.text().toStdString().c_str());
   }
}

void TestDelete::test_case3()
{
   try
   {
      Transaction t;

      {
         QueryResult res = t.execQuery("SELECT COUNT (*) FROM testTable");

         if (res.next())
         {
            int count = -1;
            res.fetchVars(count);

            QCOMPARE(count, rowCount());
         }
         else
         {
            QFAIL(res.lastError().text().toStdString().c_str());
         }
      }

      t.deleteFrom("testTable").where("a=?", 1);

      {
         QueryResult res = t.execQuery("SELECT COUNT (*) FROM testTable");

         if (res.next())
         {
            int count = -1;
            res.fetchVars(count);

            QCOMPARE(count, rowCount() - 1);
         }
         else
         {
            QFAIL(res.lastError().text().toStdString().c_str());
         }
      }

      t.deleteFrom("testTable").where("a=? OR b=?", 1, 5);

      {
         QueryResult res = t.execQuery("SELECT COUNT (*) FROM testTable");

         if (res.next())
         {
            int count = -1;
            res.fetchVars(count);

            QCOMPARE(count, rowCount() - 2);
         }
         else
         {
            QFAIL(res.lastError().text().toStdString().c_str());
         }
      }

      QCOMPARE(t.commited(), false);
   }
   catch (const DBException &e)
   {
      QFAIL(e.lastError.text().toStdString().c_str());
   }
}

void TestDelete::test_case4()
{
   try
   {
      Transaction t;

      {
         QueryResult res = t.execQuery("SELECT COUNT (*) FROM testTable");

         if (res.next())
         {
            int count = -1;
            res.fetchVars(count);

            QCOMPARE(count, rowCount());
         }
         else
         {
            QFAIL(res.lastError().text().toStdString().c_str());
         }
      }

      t.deleteFrom("testTable").where("a=?", 1);

      {
         QueryResult res = t.execQuery("SELECT COUNT (*) FROM testTable");

         if (res.next())
         {
            int count = -1;
            res.fetchVars(count);

            QCOMPARE(count, rowCount() - 1);
         }
         else
         {
            QFAIL(res.lastError().text().toStdString().c_str());
         }
      }

      t.deleteFrom("testTable").where("a=? OR b=?", 1, 5);

      {
         QueryResult res = t.execQuery("SELECT COUNT (*) FROM testTable");

         if (res.next())
         {
            int count = -1;
            res.fetchVars(count);

            QCOMPARE(count, rowCount() - 2);
         }
         else
         {
            QFAIL(res.lastError().text().toStdString().c_str());
         }
      }

      t.commit();

      QCOMPARE(t.commited(), true);
   }
   catch (const DBException &e)
   {
      QFAIL(e.lastError.text().toStdString().c_str());
   }

   Transaction t2;
   QueryResult res = t2.execQuery("SELECT COUNT (*) FROM testTable");

   if (res.next())
   {
      int count = -1;
      res.fetchVars(count);

      QCOMPARE(count, rowCount() - 2);
   }

   QCOMPARE(t2.commited(), false);
}

QTEST_APPLESS_MAIN(TestDelete)

#include "tst_testdelete.moc"
