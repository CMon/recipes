!include(../../config.pri):error("base config file (config.pri) not available")

QT       += core network
QT       -= gui

TARGET = server

CONFIG   += console
CONFIG   -= app_bundle
TEMPLATE = app

SOURCES += \
    main.cpp
