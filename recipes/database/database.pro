!include(../config.pri):error("Could not find config.pri")

QT += sql
QT -= gui

HEADERS += \
    dbrecipe.h \

TARGET = database_lib

useLibs($$RECIPES_ROOT/lib, common_lib)
useLibs(cflib_db)
lib($$RECIPES_ROOT/lib)
