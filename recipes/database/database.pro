!include(../../config.pri):error("Could not find config.pri")

QT += sql
QT -= gui

TEMPLATE = lib
TARGET = database_lib
DESTDIR=$${LIB_OUT}

HEADERS += \
    dbrecipe.h \
    dbuser.h \

SOURCES += \
    dbrecipe.cpp \
    dbuser.cpp \

useLibs($${LIB_OUT}, common_lib)
