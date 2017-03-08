#-------------------------------------------------
#
# Project created by QtCreator 2017-02-22T15:52:26
#
#-------------------------------------------------

QT += widgets


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = View
TEMPLATE = app

#
QT_CONFIG -= no-pkg-config
CONFIG += link_pkgconfig
PKGCONFIG += opencv
CONFIG += c++11
#LIBS += -ltbb
#

SOURCES += mainwindow.cpp videoitem.cpp

HEADERS  += mainwindow.h videoitem.h

FORMS    += mainwindow.ui
