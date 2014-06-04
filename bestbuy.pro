#-------------------------------------------------
#
# Project created by QtCreator 2014-05-27T17:00:55
#
#-------------------------------------------------

QT       += core network sql

QT       -= gui

TARGET = bestbuy
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    jobscheduler.cpp \
    bestbuy.cpp \
    inserter.cpp \
    requester.cpp \
    menu.cpp \
    merchandise.cpp \
    utils.cpp

HEADERS += \
    jobscheduler.h \
    bestbuy.h \
    inserter.h \
    requester.h \
    menu.h \
    merchandise.h \
    utils.h

QMAKE_CXXFLAGS += -std=c++11
