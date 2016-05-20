#-------------------------------------------------
#
# Project created by QtCreator 2016-05-08T21:13:44
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = bj
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    editwidget.cpp \
    notetree.cpp \
    notewidget.cpp

HEADERS  += mainwindow.h \
    editwidget.h \
    notetree.h \
    notewidget.h

FORMS    += mainwindow.ui

RESOURCES += \
    res.qrc
