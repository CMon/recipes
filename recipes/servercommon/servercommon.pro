!include(../config.pri):error("Could not find config.pri")

QT -= gui

HEADERS += \
    clientinfocache.h

TARGET = servercommon_lib

lib($$RECIPES_ROOT/lib)
