#-------------------------------------------------
#
# Project created by QtCreator 2016-05-02T17:38:49
#
#-------------------------------------------------

QT       += core network

QT       -= gui

TARGET = Direct2_server
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    server.cpp \
    waypoint.cpp \
    flightplan.cpp

HEADERS += \
    server.h \
    waypoint.h \
    flightplan.h
