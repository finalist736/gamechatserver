#-------------------------------------------------
#
# Project created by QtCreator 2014-08-10T00:16:07
#
#-------------------------------------------------

QT       += core network sql

QT       -= gui

TARGET = gameChatServer
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

unix {
    CONFIG    += link_pkgconfig
    PKGCONFIG += eventdispatcher_libevent
}


SOURCES += main.cpp \
    connection.cpp \
    mdb.cpp \
    server.cpp \
    settings.cpp \
    authorizer.cpp \
    areal.cpp \
    absconnection.cpp \
    channel.cpp \
    arealkeeper.cpp \
    commands.cpp

HEADERS += \
    connection.h \
    mdb.h \
    server.h \
    settings.h \
    authorizer.h \
    areal.h \
    absconnection.h \
    channel.h \
    sqlqueries.h \
    arealkeeper.h \
    commands.h
