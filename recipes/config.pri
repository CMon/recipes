include(../cflib/include.pri)

RECIPES_ROOT=$$PWD

exists($${RECIPES_ROOT}/local.pri) {
    include($${RECIPES_ROOT}/local.pri)
}

INCLUDEPATH += $${EXT_INCLUDES} $${RECIPES_ROOT} build/gen .
