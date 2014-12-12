!include(../config.pri):error("Could not find config.pri")

QT -= gui

HEADERS += \
    util.h \
    recipe.h \
    ingredient.h \
    user.h \
    category.h \
    unit.h \
    portion.h \
    permission.h \
    waiter.h \
    locale2string.h \
    log.h

TARGET = common_lib

useLibs(cflib_serialize cflib_util cflib_libev)
lib($$RECIPES_ROOT/lib)
serializeGen()
