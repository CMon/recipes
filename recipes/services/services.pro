!include(../../config.pri):error("Could not find config.pri")

QT -= gui
QT += network

TEMPLATE = lib
TARGET = services_lib
DESTDIR=$${LIB_OUT}

HEADERS += \
    logCategory.h \
    recipeservice.h \
    userservice.h \

SOURCES += \
    recipeservice.cpp \
    userservice.cpp \

useLibs($${LIB_OUT}, servercommon_lib)
