!include(../../config.pri):error("base config file (config.pri) not available")

QT       += core

HEADERS += \
    dbrecipe.h \
    dbgeneral.h

TARGET = database_lib
initLib()
