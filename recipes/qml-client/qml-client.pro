!include(../../config.pri):error("Could not find config.pri")


QT += qml quick widgets network
TEMPLATE = app
DESTDIR=$${BIN_OUT}

SOURCES += \
    main.cpp \
    servicemapper.cpp \

HEADERS += \
    logcategory.h \
    servicemapper.h \

RESOURCES += \
    qml.qrc

useLibs($${LIB_OUT}, common_lib)

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)
