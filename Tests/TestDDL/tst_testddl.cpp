#include <QtTest>
#include "EasyQtSql.h"

using namespace EasyQtSql;

class TestDDL : public QObject
{
   Q_OBJECT

public:
   TestDDL() {}
   ~TestDDL(){}

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

};

void TestDDL::initTestCase()
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
}

void TestDDL::cleanupTestCase()
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

void TestDDL::test_case1()
{   
   QVERIFY_EXCEPTION_THROWN({
                               Transaction t;

                               PreparedQuery query = t.prepare("SELECT * FROM table1"); //there is no table1, so exception expected

                               QueryResult res = query.exec();

                            }, DBException);
}

void TestDDL::test_case2()
{   
   try
   {
      Transaction t;

      t.execNonQuery("CREATE TABLE table2 (a int, b int, c int, d text)"); //create table2

      PreparedQuery query = t.prepare("SELECT * FROM table2"); //...and select from (empty) table2

      QueryResult res = query.exec();

      //transaction is uncommited, so table2 will be removed after rollback
   }
   catch (const DBException &e)
   {
      QFAIL(e.lastError.text().toStdString().c_str());
   }
}

void TestDDL::test_case3()
{   
   try
   {
      Transaction t;

      t.execNonQuery("CREATE TABLE table2 (a int, b int, c int, d text)");

      //transaction is uncommited
   }
   catch (const DBException &e)
   {
      QFAIL(e.lastError.text().toStdString().c_str());
   }
}

void TestDDL::test_case4()
{
   try
   {
      Transaction t;

      t.execNonQuery("CREATE TABLE table2 (a int, b int, c int, d text)"); //create table2

      PreparedQuery query = t.prepare("SELECT * FROM table2");

      QueryResult res = query.exec();

      t.commit(); // commit transaction (table2 created and commited)
   }
   catch (const DBException &e)
   {
      QFAIL(e.lastError.text().toStdString().c_str());
   }
}

void TestDDL::test_case5()
{
   QVERIFY_EXCEPTION_THROWN({

                               Transaction t;
                               t.execNonQuery("CREATE TABLE table2 (a int, b int, c int, d text)"); //cant create existed table2

                            }, DBException);
}

void TestDDL::test_case6()
{
   QVERIFY_EXCEPTION_THROWN({

                               Transaction t;
                               t.execNonQuery("DROP TABLE table4"); //can't drop table4

                            }, DBException);
}

void TestDDL::test_case7()
{
   try
   {
      Transaction t;

      t.execNonQuery("DROP TABLE table2"); //can drop table2

      t.commit(); // commit transaction
   }
   catch (const DBException &e)
   {
      QFAIL(e.lastError.text().toStdString().c_str());
   }
}

void TestDDL::test_case8()
{
   QVERIFY_EXCEPTION_THROWN({

                               Transaction t;
                               t.execNonQuery("DROP TABLE table2"); //there is no table2

                            }, DBException);
}


QTEST_APPLESS_MAIN(TestDDL)

#include "tst_testddl.moc"
