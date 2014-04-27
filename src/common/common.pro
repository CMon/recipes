!include(../../config.pri):error("base config file (config.pri) not available")

QT       += core

HEADERS += \
    log.h \
    util.h \
    recipe.h \
    ingredient.h \
    user.h \
    category.h \
    unit.h \
    portion.h

TARGET = common_lib
initLib()
