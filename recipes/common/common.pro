!include(../config.pri):error("Could not find config.pri")

QT -= gui

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
lib($$RECIPES_ROOT/lib)
