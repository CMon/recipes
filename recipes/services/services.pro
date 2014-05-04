!include(../config.pri):error("Could not find config.pri")

QT -= gui

HEADERS += \
    userservice.h

TARGET = services_lib

useLibs(cflib_crypt)
lib($$RECIPES_ROOT/lib)
serializeGen()

