!include(../config.pri):error("Could not find config.pri")

QT       += network
QT       -= gui

TARGET = console-client

CONFIG   += console

SOURCES += \
    main.cpp \
    consoleclient.cpp \
    console.cpp

HEADERS += \
    consoleclient.h \
    console.h

useLibs($$RECIPES_ROOT/lib, common_lib)
app()
