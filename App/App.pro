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
