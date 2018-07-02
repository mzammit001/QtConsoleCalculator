TEMPLATE = app

QT += core gui widgets

CONFIG += console c++14
CONFIG -= app_bundle

SOURCES += \
        main.cpp \
    confighandler.cpp

HEADERS += \
    common.h \
    confighandler.h \
    configtypeproxy.h
