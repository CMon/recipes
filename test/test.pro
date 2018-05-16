!include(../config.pri):error("Could not find config.pri")

QT += sql network testlib
QT -= gui

TEMPLATE = app
TARGET = tests
DESTDIR = $${BIN_OUT}

HEADERS += \
    database_test.h \
    service_test.h \
    testserver.h \
    testtostrings.h \

SOURCES += \
    database_test.cpp \
    service_test.cpp \
    testserver.cpp \

RESOURCES += \
    ../database.qrc

useLibs($${LIB_OUT}, services_lib database_lib common_lib servercommon_lib)
