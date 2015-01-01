#-------------------------------------------------
#
# Project created by QtCreator 2014-12-31T21:58:50
#
#-------------------------------------------------

QT       += core network

QT       -= gui

TARGET = dahlia_server
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    network_server.cpp

HEADERS += \
    network_server.h

LIBS += -lsodium -ljansson
