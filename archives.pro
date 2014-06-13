#-------------------------------------------------
#
# Project created by QtCreator 2014-06-07T20:56:46
#
#-------------------------------------------------

QT       += core network sql

QT       -= gui

TARGET = archives
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    inserter.cpp \
    requester.cpp \
    parser.cpp \
    scheduler.cpp \
    extractor.cpp \
    preprocessor.cpp \
    category.cpp \
    product.cpp

HEADERS += \
    inserter.h \
    requester.h \
    parser.h \
    scheduler.h \
    extractor.h \
    utility.h \
    preprocessor.h \
    category.h \
    product.h

QMAKE_CXXFLAGS += -std=c++11
