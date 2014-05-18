!include(../recipes/config.pri):error("Could not find config.pri")

QT += sql
QT -= gui

TARGET = tests

HEADERS += \
    database_test.h

SOURCES += \
    database_test.cpp

RESOURCES += \
    ../database.qrc

useLibs($$RECIPES_ROOT/lib, database_lib common_lib)
useLibs(cflib_db cflib_crypt cflib_util)
test()
