!include(../config.pri):error("Could not find config.pri")

TEMPLATE = app

QT += qml quick widgets network

SOURCES += \
	main.cpp

RESOURCES += \
	qml.qrc

useLibs($$RECIPES_ROOT/lib, common_lib)

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)
