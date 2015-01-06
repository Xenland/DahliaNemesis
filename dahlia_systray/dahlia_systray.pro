#-------------------------------------------------
#
# Project created by QtCreator 2014-12-28T18:07:22
#
#-------------------------------------------------

QT       += core gui sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = dahlia_systray
TEMPLATE = app


SOURCES += main.cpp\
        systray_manager.cpp \
    contact_book.cpp \
    crypto_manager.cpp \
    chat_manager.cpp \
    chat_box.cpp \
    network_manager.cpp

HEADERS  += systray_manager.h \
    contact_book.h \
    crypto_manager.h \
    chat_manager.h \
    chat_box.h \
    network_manager.h

RESOURCES += \
    systray_resource.qrc

LIBS += -lsodium -ljansson
