!include(../config.pri):error("Could not find config.pri")

QT       += network
QT       -= gui

TARGET = console-client

CONFIG   += console

SOURCES += \
    main.cpp \
    consoleclient.cpp \
    console.cpp \
    remote.cpp

HEADERS += \
    consoleclient.h \
    console.h \
    remote.h

useLibs($$RECIPES_ROOT/lib, common_lib)
app()
