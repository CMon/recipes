RECIPES_ROOT=$$PWD
RECIPES_OUT=$$shadowed($$RECIPES_ROOT)
exists($${RECIPES_ROOT}/local.pri) {
    include($${RECIPES_ROOT}/local.pri)
}

isEmpty(PREFIX) {
    TARGET_PREFIX="/usr/"
} else {
    TARGET_PREFIX=$${PREFIX}"/"
}

CONFIG += c++11

LIB_OUT = $${RECIPES_OUT}/lib
BIN_OUT = $${RECIPES_OUT}/bin

INCLUDEPATH += $${OBJECTS_DIR} $${MOC_DIR} $${UI_DIR} $${RECIPES_ROOT}
DEPENDPATH  += $${RECIPES_ROOT}

QMAKE_LIBDIR += $${LIB_OUT}

# useLibs(path, lib1 lib2 ...)
defineTest(useLibs) {
	defined(2, var) {
		path = $$1
		libs = $$2
	} else {
		error("Missuse of »useLibs«, should be something like »useLibs(<pathOfLib>, <space seperated list of libs to link to>)")
	}
	LIBS += -L$$path
	for(lib, libs) {
		LIBS += -l$$lib
	}
	export(LIBS)
}

# useSystemLibs(lib1 lib2 ...)
defineTest(useSystemLibs) {
	libs = $$1
	for(lib, libs) {
		LIBS += -l$$lib
	}
	export(LIBS)
}

# addSubdir(dir, dependandept1 dependant2 ...)
# is used to automatically add .depends stuff
defineTest(addSubdir) {
	TEMPLATE = subdirs
	export(TEMPLATE)
	dir = $$replace(1, /, _)
	eval  ($${dir}.subdir = $$1)
	export($${dir}.subdir)
	depends = $$replace(2, /, _)
	eval  ($${dir}.depends = $$depends)
	export($${dir}.depends)
	SUBDIRS += $$dir
	export(SUBDIRS)
}
