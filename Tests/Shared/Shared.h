#ifndef SHARED_H
#define SHARED_H

#include <QtCore>

struct Row
{
   int a;
   int b;
   int c;
   QString d;

private:
   Q_GADGET
   Q_PROPERTY(int a MEMBER a)
   Q_PROPERTY(int b MEMBER b)
   Q_PROPERTY(int c MEMBER c)
   Q_PROPERTY(QString d MEMBER d)
};

class TestObject : public QObject, public Row
{
   Q_OBJECT
   Q_PROPERTY(int a MEMBER a)
   Q_PROPERTY(int b MEMBER b)
   Q_PROPERTY(int c MEMBER c)
   Q_PROPERTY(QString d MEMBER d)
};


#endif // SHARED_H
