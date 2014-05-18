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
    portion.h \
    permission.h \
    waiter.h

TARGET = common_lib

useLibs(cflib_serialize cflib_util)
lib($$RECIPES_ROOT/lib)
serializeGen()