#-------------------------------------------------
#
# Project created by QtCreator 2014-02-24T22:20:58
#
#-------------------------------------------------

QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = spider_with_ui_v04
TEMPLATE = app


SOURCES += main.cpp\
        spider.cpp \
    utility.cpp

HEADERS  += spider.h \
    product.h \
    packet.h \
    category.h \
    utility.h

QMAKE_CXXFLAGS += -std=c++11
