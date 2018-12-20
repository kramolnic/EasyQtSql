#include <QCoreApplication>
#include "EasyQtSql.h"
#include "TestObject.h"

using namespace EasyQtSql;

int main(int argc, char *argv[])
{
   QCoreApplication a(argc, argv);

   qDebug() << "begin";

   SqlFactory *factory = SqlFactory::getInstance()->config(SqlFactory::DBSetting::sqliteInmemory());

   QSqlDatabase sdb = factory->getDatabase();

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

      t.first("SELECT 1", [](const QueryResult &row){
         qDebug() << row.scalar<bool>() << row.scalar<QDate>();
      });

      t.each("SELECT * FROM table1", [](const QueryResult &row){
         qDebug() << row.toMap();
      });

      Util::each(query.exec(1, 2), [](const QueryResult &row){
         qDebug() << row.toMap();
      });

      int i = t.scalar<int>("SELECT 1");
      int j = t.scalar("SELECT 1").toInt();

      qDebug() << i << j;

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
