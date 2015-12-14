#-------------------------------------------------
#
# Project created by QtCreator 2015-11-27T01:26:53
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = JarvisServer
TEMPLATE = app

INCLUDEPATH+= ../JarvisFirmware/


SOURCES += main.cpp\
        gjarvisgui.cpp \
    sjarvistcpserver.cpp \
    sjarvistcpclient.cpp \
    sjarvisnodeserver.cpp \
    sjarvis.cpp \
    sjarvisnode.cpp \
    gwidgettxrx.cpp \
    gwidgetnode.cpp \
    gnodetestwidget.cpp \
    sjarvisnodecomponent.cpp \
    gnodecomponentwidget.cpp \
    gblinkwidget.cpp \
    gsimplegraph.cpp

HEADERS  += gjarvisgui.h \
    ../JarvisFirmware/jarvisParser.h \
    ../JarvisFirmware/jarvisProtocol.h \
    sjarvistcpserver.h \
    sjarvisnodeserver.h \
    sjarvis.h \
    sjarvisnode.h \
    gwidgettxrx.h \
    gwidgetnode.h \
    gnodetestwidget.h \
    arduinoCompat.h \
    sjarvistcpclient.h \
    sjarvisnodecomponent.h \
    gnodecomponentwidget.h \
    gblinkwidget.h \
    gsimplegraph.h

FORMS    += gjarvisgui.ui \
    gwidgetnode.ui \
    gnodetestwidget.ui \
    gnodecomponentwidget.ui \
    gblinkwidget.ui \
    gsimplegraph.ui
