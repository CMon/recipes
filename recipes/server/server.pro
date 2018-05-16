!include(../../config.pri):error("Could not find config.pri")

CONFIG   += console
CONFIG   -= app_bundle

QT += network sql
QT -= gui

TEMPLATE = app
TARGET = server
DESTDIR=$${BIN_OUT}

SOURCES += \
    main.cpp

useLibs($${LIB_OUT}, services_lib database_lib common_lib servercommon_lib)
