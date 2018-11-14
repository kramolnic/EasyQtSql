#include "TestObject.h"
#include "EasyQtSql.h"

using namespace EasyQtSql;

TestObject::TestObject(QObject *parent) : QObject(parent)
{

}

int TestObject::a() const
{
   return m_a;
}

void TestObject::setA(int a)
{
   m_a = a;
}

int TestObject::b() const
{
   return m_b;
}

void TestObject::setB(int b)
{
   m_b = b;
}

int TestObject::c() const
{
   return m_c;
}

void TestObject::setC(int c)
{
   m_c = c;
}

QString TestObject::d() const
{
   return m_d;
}

void TestObject::setD(const QString &d)
{
   m_d = d;
}
