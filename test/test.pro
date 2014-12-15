!include(../recipes/config.pri):error("Could not find config.pri")

QT += sql network
QT -= gui

TARGET = tests

HEADERS += \
	database_test.h \
	service_test.h \
	testserver.h

SOURCES += \
	database_test.cpp \
	service_test.cpp \
	testserver.cpp

RESOURCES += \
	../database.qrc

useLibs($$RECIPES_ROOT/lib, services_lib database_lib common_lib)
useLibs(cflib_db cflib_http cflib_crypt cflib_serialize cflib_util cflib_libev)
test()
