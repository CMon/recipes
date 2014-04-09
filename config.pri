RECEIPTS_ROOT=$$PWD

exists($${RECEIPTS_ROOT}/local.pri) {
    include($${RECEIPTS_ROOT}/local.pri)
}

CONFIG(debug, debug|release)|CONFIG(DebugBuild) {
        CONFIG -= debug release
        CONFIG += debug
        DEBUG_OR_RELEASE = debug
} else {
        CONFIG -= debug release
        CONFIG += release
        DEBUG_OR_RELEASE = release
        DEFINES += NDEBUG
}

CONFIG -= warn_off
CONFIG += warn_on

OBJECTS_DIR  = build/$${DEBUG_OR_RELEASE}
MOC_DIR      = build/gen/moc
UI_DIR       = build/gen/ui
BIN_DIR      = $${RECEIPTS_ROOT}/bin/$${DEBUG_OR_RELEASE}
LIB_DIR      = $${RECEIPTS_ROOT}/libs/$${DEBUG_OR_RELEASE}
DESTDIR      = $${BIN_DIR}

MOC_DIR      = build/gen/moc
UI_DIR       = build/gen/ui
SER_DIR      = build/gen/ser
RCC_DIR      = build/gen/rcc

win32 {
    LIB_PREFIX  =
    LIB_POSTFIX = lib
    EXE_POSTFIX = .exe
} else {
    LIB_PREFIX  = lib
    LIB_POSTFIX = a
    EXE_POSTFIX =
}

INCLUDEPATH += $${EXT_INCLUDES} $${RECEIPTS_ROOT} build/gen . $${RECEIPTS_ROOT}/src
DEPENDPATH  += $${RECEIPTS_ROOT}

QMAKE_LIBDIR += $${LIB_DIR} $${EXT_LIB_DIR}

QMAKE_DISTCLEAN += $${RECEIPTS_ROOT}/bin/debug/*
QMAKE_DISTCLEAN += $${RECEIPTS_ROOT}/bin/release/*

defineTest(initApp) {
    TEMPLATE = app

    for(lib, 1) {
        LIBS += -l$${lib}
        POST_TARGETDEPS += $${LIB_DIR}/$${LIB_PREFIX}$${lib}.$${LIB_POSTFIX}
    }
    LIBS += $$EXT_LIBS

    export(TEMPLATE)
    export(LIBS)
    export(POST_TARGETDEPS)

    !win32:release:!no_strip:console {
        QMAKE_POST_LINK += strip $${DESTDIR}/$${TARGET} ;
        export(QMAKE_POST_LINK)
    }

    macx:console {
        CONFIG -= app_bundle
        export(CONFIG)
    }
}

defineTest(initLib) {
    TEMPLATE = lib

    for(hFile, HEADERS) {
        cppFile = $$replace(hFile,\\.h,.cpp)
        exists($$cppFile):SOURCES *= $$cppFile
    }

    CONFIG += staticlib
    DESTDIR = $${LIB_DIR}

    export(TEMPLATE)
    export(SOURCES)
    export(CONFIG)
    export(DESTDIR)
}

defineTest(addSubdirs) {
    for(subdirs, 1) {
        entries = $$files($$subdirs)
        for(entry, entries) {
            name = $$replace(entry, [/\\\\], _)
            subdir_path = $$replace(entry, \\\\, /)
            SUBDIRS += $$name
            eval ($${name}.subdir = $$subdir_path)
            for(dep, 2):eval ($${name}.depends += $$replace(dep, [/\\\\], _))
            export ($${name}.subdir)
            export ($${name}.depends)
        }
    }
    export (SUBDIRS)
}
