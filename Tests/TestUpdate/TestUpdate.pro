QT += testlib sql
QT -= gui

include(../../EasyQtSql/EasyQtSql.pri)

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  tst_testupdate.cpp
