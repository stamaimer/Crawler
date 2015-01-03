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
    product.h \
    rapidjson/error/en.h \
    rapidjson/error/error.h \
    rapidjson/internal/biginteger.h \
    rapidjson/internal/diyfp.h \
    rapidjson/internal/dtoa.h \
    rapidjson/internal/ieee754.h \
    rapidjson/internal/itoa.h \
    rapidjson/internal/meta.h \
    rapidjson/internal/pow10.h \
    rapidjson/internal/stack.h \
    rapidjson/internal/strfunc.h \
    rapidjson/internal/strtod.h \
    rapidjson/msinttypes/inttypes.h \
    rapidjson/msinttypes/stdint.h \
    rapidjson/allocators.h \
    rapidjson/document.h \
    rapidjson/encodedstream.h \
    rapidjson/encodings.h \
    rapidjson/filereadstream.h \
    rapidjson/filestream.h \
    rapidjson/filewritestream.h \
    rapidjson/memorybuffer.h \
    rapidjson/memorystream.h \
    rapidjson/prettywriter.h \
    rapidjson/rapidjson.h \
    rapidjson/reader.h \
    rapidjson/stringbuffer.h \
    rapidjson/writer.h

QMAKE_CXXFLAGS += -std=c++11
