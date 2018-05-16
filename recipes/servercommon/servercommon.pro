!include(../../config.pri):error("Could not find config.pri")

QT -= gui

TEMPLATE = lib
TARGET = servercommon_lib
DESTDIR=$${LIB_OUT}

HEADERS += \
    clientinfocache.h
