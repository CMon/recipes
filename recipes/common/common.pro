!include(../../config.pri):error("Could not find config.pri")

QT -= gui
TEMPLATE = lib
TARGET = common_lib
DESTDIR=$${LIB_OUT}

HEADERS += \
    category.h \
    ingredient.h \
    jsonserialize.h \
    locale2string.h \
    log.h \
    permission.h \
    portion.h \
    recipe.h \
    unit.h \
    user.h \
    util.h \
    waiter.h \

SOURCES += \
    category.cpp \
    ingredient.cpp \
    locale2string.cpp \
    log.cpp \
    permission.cpp \
    portion.cpp \
    recipe.cpp \
    unit.cpp \
    user.cpp \
    util.cpp \
    waiter.cpp \
