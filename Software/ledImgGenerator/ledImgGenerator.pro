#-------------------------------------------------
#
# Project created by QtCreator 2016-04-05T15:46:46
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

INCLUDEPATH+= ../JarvisServer/common \

TARGET = ledImgGenerator
TEMPLATE = app


SOURCES += main.cpp \
    ../JarvisServer/common/qimageselectionwidget.cpp

HEADERS  += \
    ../JarvisServer/common/qimageselectionwidget.h

FORMS    += \
    ../JarvisServer/common/qimageselectionwidget.ui
