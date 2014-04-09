!include(../../config.pri):error("base config file (config.pri) not available")

QT       += core network
QT       -= gui

TARGET = console-client

CONFIG   += console
CONFIG   -= app_bundle
TEMPLATE = app

SOURCES += \
    main.cpp \
    consoleclient.cpp \
    console.cpp

HEADERS += \
    consoleclient.h \
    console.h

initApp(common_lib)
