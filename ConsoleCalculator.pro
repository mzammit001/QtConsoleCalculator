TEMPLATE = app

QT += core gui widgets concurrent

CONFIG += console c++17
CONFIG -= app_bundle

SOURCES += \
        main.cpp \
    confighandler.cpp \
    expressionlexer.cpp \
    mainwindow.cpp \
    parsetablegenerator.cpp \
    parsetable.cpp

HEADERS += \
    common.h \
    confighandler.h \
    configtypeproxy.h \
    expressionlexer.h \
    mainwindow.h \
    textresource.h \
    parsetablegenerator.h \
    parsetable.h
