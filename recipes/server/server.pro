!include(../config.pri):error("Could not find config.pri")

QT       += core network
QT       -= gui

TARGET = server

CONFIG   += console
CONFIG   -= app_bundle
TEMPLATE = app

SOURCES += \
    main.cpp

useLibs($$RECIPES_ROOT/lib, common_lib database_lib services_lib)
