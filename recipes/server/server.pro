!include(../config.pri):error("Could not find config.pri")

QT       += network sql
QT       -= gui

TARGET = server

CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += \
    main.cpp

useLibs(cflib_db cflib_http cflib_crypt cflib_serialize cflib_util)

useLibs($$RECIPES_ROOT/lib, common_lib database_lib services_lib)

app()
