!include(../../config.pri):error("Could not find config.pri")

QT += network
QT -= gui

TEMPLATE = app
TARGET = console-client
CONFIG = console
DESTDIR=$${BIN_OUT}

SOURCES += \
    console.cpp \
    consoleclient.cpp \
    main.cpp \
    remote.cpp

HEADERS += \
    console.h \
    consoleclient.h \
    logCategory.h
    remote.h \

useLibs($${LIB_OUT}, common_lib)
