# EasyQtSql
[EasyQtSql](https://kramolnic.github.io/EasyQtSql/) is a lightweight header-only C++ library for quick and easy SQL querying with QtSql classes.

## Getting Started
### Prerequisites
You need to install:
* C++11 compiler
* Qt5 library
* Qt Creator 4.7.2 - recommended (for automated test running)
* QtSql module with the required SQL database drivers compiled / installed

The library was compiled and tested with:
* MS Visual C++ Compiler 14.0 / Qt5.6 and Qt5.7
* Linux Kubuntu GCC compiler / Qt5.9

### Step 1: Create Qt application project

You can use QtCreator to create a simple minimal console application with a single main.cpp file.
Or use a ready-made application where you want to add database support.

### Step 2: Add QtSql support
Open *.pro file and add QtSql module (QT += sql):
```
QT += sql
QT -= gui
CONFIG += c++11
TARGET = App
CONFIG += console
CONFIG -= app_bundle
TEMPLATE = app
```
### Step 3: Include EasyQtSql library
To use the library you need to include single header file (EasyQtSql.h) in your source code. So you need to put the library header files (EasyQtSql folder) in an accessible place next to your project and to specify path to the EasyQtSql folder with INCLUDEPATH directive. You can just include EasyQtSql.pri inside your *.pro file:
```
QT += sql
QT -= gui
include(../EasyQtSql/EasyQtSql.pri)
CONFIG += c++11
TARGET = App
CONFIG += console
CONFIG -= app_bundle
TEMPLATE = app
#INCLUDEPATH += ../EasyQtSql
SOURCES += main.cpp \
    TestObject.cpp
HEADERS += \
    TestObject.h     
```
### Step 4: Include the library header file and start using it
Include EasyQtSql.h header file and start using the library.
```c++
#include <QCoreApplication>
#include "EasyQtSql.h"
#include "TestObject.h"
using namespace EasyQtSql;
int main(int argc, char *argv[])
{
   QCoreApplication a(argc, argv);
   qDebug() << "begin";
   QSqlDatabase sdb = QSqlDatabase::addDatabase("QSQLITE");
   sdb.setDatabaseName(":memory:");
   try
   {
      Transaction t(sdb);
      t.execNonQuery("CREATE TEMP TABLE IF NOT EXISTS table1 (a int, b int, c int, d text)");
      //multi insert
      NonQueryResult res1 = t.insertInto("table1 (a, b, c, d)")
         .values(1, 2, 3, 7)
         .values(2, 3, 4, "test")
         .values(2, 3, 4, 8)
         .exec();
      qDebug() << res1.lastQuery();
      //delete with condition
      //t.deleteFrom("table1").where("a = ? AND b = ?", 1, 2);
      //delete without condition
      //t.deleteFrom("table1").exec();
      //delete with constant condition
      //t.deleteFrom("table1").where("1=0");
      PreparedQuery query = t.prepare("SELECT a, b, c, d FROM table1 WHERE a = ? AND b = ?");
      QueryResult res = query.exec(1, 2);
      qDebug() << res.executedQuery() << res.boundValue(0) << res.boundValue(1);
      while(res.next())
      {
         QVariantMap map = res.toMap();
         qDebug() << map;
      }
      res = query.exec(2, 3);
      qDebug() << res.executedQuery() << res.boundValue(0) << res.boundValue(1);
      while(res.next())
      {
         QStringList list = res.toStringList();
         qDebug() << list;
         TestObject obj;
         int a;
         bool b;
         double c;
         QVariant d;
         QByteArray arr;
         res.fetchVars(a, b, c, d, arr);
         res.fetchObject(obj);
         qDebug() << a << b << c << d;
      }
      NonQueryResult res2 = t.update("table1")
         .set("a", 123)
         .set("b", 333)
         .set(QVariantMap{{"a", 1}, {"b", 2}})
         .where("c=? AND d=?", 3, 4);
      qDebug() << res2.lastQuery() << res2.executedQuery();
      t.commit();
      qDebug() << "end";
   }
   catch (const DBException &e)
   {
      qDebug() << e.lastError << e.lastQuery;
   }
   qDebug() << "done";
   return a.exec();
}
```
## Running the tests
Tests are implemented with [QtTest](http://doc.qt.io/archives/qt-5.7/qttest-index.html) module. To run the tests you can use Qt Creator. 

## Built With

* [Qt](https://www.qt.io/) - Qt | Cross-platform software development for embedded &amp; desktop
* [QtSql](http://doc.qt.io/archives/qt-5.7/qtsql-index.html) - Qt SQL is an essential module which provides support for SQL databases

## Authors

* **kramolnic** - *Initial work* - [https://github.com/kramolnic](https://github.com/kramolnic)

## License

This project is licensed under the [MIT License](https://github.com/kramolnic/EasyQtSql/blob/master/EasyQtSql/License.txt).

