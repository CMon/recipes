!include(../config.pri):error("Could not find config.pri")

QT += sql
QT -= gui

HEADERS += \
    dbrecipe.h \
    dbuser.h

TARGET = database_lib

useLibs($$RECIPES_ROOT/lib, common_lib)
useLibs(cflib_db cflib_util)
lib($$RECIPES_ROOT/lib)
