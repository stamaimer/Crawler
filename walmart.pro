#-------------------------------------------------
#
# Project created by QtCreator 2014-04-10T15:11:41
#
#-------------------------------------------------

QT       += core network sql

QT       -= gui

TARGET = walmart
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    jobscheduler.cpp \
    requester.cpp \
    walmart.cpp

HEADERS += \
    jobscheduler.h \
    requester.h \
    walmart.h \
    utils.h \
    menu.h \
    merchandise.h
