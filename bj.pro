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
    notewidget.cpp \
    docwidget.cpp \
    doctree.cpp \
    docdetail.cpp \
    docimport_dialog.cpp

HEADERS  += mainwindow.h \
    editwidget.h \
    notetree.h \
    notewidget.h \
    docwidget.h \
    doctree.h \
    docdetail.h \
    docparam.h \
    docimport_dialog.h

FORMS    +=

RESOURCES += \
    res.qrc
