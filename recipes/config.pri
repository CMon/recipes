RECIPES_ROOT=$$PWD"/../"

exists($${RECIPES_ROOT}/local.pri) {
    include($${RECIPES_ROOT}/local.pri)
}

include(../cflib/include.pri)

INCLUDEPATH += $${EXT_INCLUDES} $${RECIPES_ROOT} build/gen .

CONFIG += c++11
