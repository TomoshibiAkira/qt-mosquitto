#-------------------------------------------------
#
# Project created by QtCreator 2015-05-04T23:34:34
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qt-mosquitto
TEMPLATE = app

LIBS += -lmosquittopp
LIBS += -lmosquitto


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h \
    qtmosq.h

FORMS    += mainwindow.ui
