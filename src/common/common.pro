!include(../../config.pri):error("base config file (config.pri) not available")

QT       += core

HEADERS += \
    log.h \
    util.h

TARGET = common_lib
initLib()
