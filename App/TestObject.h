#ifndef TESTOBJECT_H
#define TESTOBJECT_H

#include <QObject>

class TestObject : public QObject
{
   Q_OBJECT
   Q_PROPERTY(int a READ a WRITE setA)
   Q_PROPERTY(int b READ b WRITE setB)
   Q_PROPERTY(int c READ c WRITE setC)
   Q_PROPERTY(QString d READ d WRITE setD)

public:
   explicit TestObject(QObject *parent = nullptr);

   int a() const;
   void setA(int a);

   int b() const;
   void setB(int b);

   int c() const;
   void setC(int c);

   QString d() const;
   void setD(const QString &d);

private:
   int m_a;
   int m_b;
   int m_c;
   QString m_d;

signals:

public slots:
};

#endif // TESTOBJECT_H
