#-------------------------------------------------
#
# Project created by QtCreator 2014-03-23T10:27:44
#
#-------------------------------------------------

QT       += core network

QT       -= gui

TARGET = synnex
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    synnex.cpp \
    jobscheduler.cpp \
    product.cpp \
    category.cpp \
    subcategory.cpp

HEADERS += \
    synnex.h \
    jobscheduler.h \
    product.h \
    category.h \
    subcategory.h
