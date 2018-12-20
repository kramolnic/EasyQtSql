#ifndef EASYQTSQL_SQLFACTORY_H
#define EASYQTSQL_SQLFACTORY_H

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

class SqlFactory
{
public:

   static SqlFactory *getInstance()
   {
      static SqlFactory instance;
      return &instance;
   }

   class ThreadDBPool;

   struct DBSetting
   {
      friend class ThreadDBPool;

      DBSetting()
      { }

      DBSetting(const QString &type, const QString &host, int port, const QString &username, const QString &password, const QString &dbname)
         : type(type), port(port), host(host), username(username), password(password), dbName(dbname)
      { }

      DBSetting(const QString &type, const QString &connectionString)
         : type(type), dbName(connectionString)
      { }

      static DBSetting sqliteInmemory()
      {
         return DBSetting("QSQLITE", ":memory:");
      }

      QString getType() const
      {
         return type.isEmpty() ? "QODBC" : type;
      }

   private:
      QString  type;
      QVariant port;
      QString  host;
      QString  username;
      QString  password;
      QString  dbName;
   };

   class ThreadDBPool
   {
   public:

      ~ThreadDBPool()
      {
         for (const QString &uniqueConnName : m_connNameMap.values())
         {
            {
               QSqlDatabase db = QSqlDatabase::database(uniqueConnName);
               if (db.isOpen())
               {
                  db.close();
               }
            }

            QSqlDatabase::removeDatabase(uniqueConnName);
         }
      }

      void addConnection(const DBSetting &settings, const QString &connectionName)
      {
         const QString uniqueConnectionName = connectionName + QUuid::createUuid().toString();

         m_connNameMap.insert(connectionName, uniqueConnectionName);

         QSqlDatabase db = QSqlDatabase::addDatabase(settings.getType(), uniqueConnectionName);

         db.setDatabaseName(settings.dbName);
         db.setHostName(settings.host);
         db.setUserName(settings.username);
         db.setPassword(settings.password);

         if (settings.port.isValid())
         {
            db.setPort(settings.port.toInt());
         }
      }

      bool connectionExists(const QString &connectionName) const
      {
         return m_connNameMap.contains(connectionName);
      }

      QSqlDatabase getDatabase(const QString &connectionName) const
      {
         const QString &uniqueConnectionName = m_connNameMap.value(connectionName);

         QSqlDatabase db = QSqlDatabase::database(uniqueConnectionName);

         if (!db.isOpen())
         {
            if (!db.open())
            {
               qCritical() << db.lastError().text();
            }
         }

         return db;
      }

   private:
      QMap<QString, QString> m_connNameMap;
   };

   SqlFactory *config(const DBSetting &settings, const QString &connectionName = QSqlDatabase::defaultConnection)
   {
      QMutexLocker locker(&mutex);

      m_settings.insert(connectionName, settings);

      return this;
   }

   QSqlDatabase getDatabase(const QString &connectionName = QSqlDatabase::defaultConnection)
   {
      QMutexLocker locker(&mutex);

      if (m_settings.contains(connectionName))
      {
         ThreadDBPool *threadDbPool = nullptr;

         if (!m_dbPool.hasLocalData())
         {
            threadDbPool = new ThreadDBPool(); //new pool for current thread

            m_dbPool.setLocalData(threadDbPool);
         }
         else
         {
            threadDbPool = m_dbPool.localData();
         }

         if (threadDbPool)
         {
            if (!threadDbPool->connectionExists(connectionName)) //if no connection created for current thread
            {
               const DBSetting &settings = m_settings[connectionName];

               threadDbPool->addConnection(settings, connectionName); //create new connection with specified settings for current thread
            }

            return threadDbPool->getDatabase(connectionName);
         }
      }

      return QSqlDatabase();
   }

private:
   QMutex mutex;

   QMap<QString, DBSetting> m_settings;
   QThreadStorage<ThreadDBPool*> m_dbPool;

   QString m_defaultConnName;

private:
   SqlFactory()
   {}
};


#endif // EASYQTSQL_SQLFACTORY_H
