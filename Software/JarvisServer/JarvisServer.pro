#-------------------------------------------------
#
# Project created by QtCreator 2015-11-27T01:26:53
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = JarvisServer
TEMPLATE = app

INCLUDEPATH+= ../JarvisFirmware/ \
                common/ \
                jarvisServer/


SOURCES += \
    common/gblinkwidget.cpp \
    common/gnodecomponentwidget.cpp \
    common/gnodetestwidget.cpp \
    common/gsimplegraph.cpp \
    common/gwidgetnode.cpp \
    common/gwidgettxrx.cpp \
    common/qimageselectionwidget.cpp \
    common/sjarvisnode.cpp \
    common/sjarvisnodecomponent.cpp \
    common/sjarvistcpclient.cpp \
    jarvisServer/gjarvisgui.cpp \
    jarvisServer/main.cpp \
    jarvisServer/sjarvis.cpp \
    jarvisServer/sjarvisnodeserver.cpp \
    jarvisServer/sjarvistcpserver.cpp \
    jarvisServer/gjarvisserverwidget.cpp \
    jarvisServer/gnodesimplewidget.cpp \
    jarvisServer/qconnectdialog.cpp \
    common/jarvisnodetestapp.cpp

HEADERS += \
    ../JarvisFirmware/jarvisProtocol.h \
    common/arduinoCompat.h \
    common/gblinkwidget.h \
    common/gnodecomponentwidget.h \
    common/gnodetestwidget.h \
    common/gsimplegraph.h \
    common/gwidgetnode.h \
    common/gwidgettxrx.h \
    common/qimageselectionwidget.h \
    common/sjarvisnode.h \
    common/sjarvisnodecomponent.h \
    common/sjarvistcpclient.h \
    jarvisServer/gjarvisgui.h \
    jarvisServer/sjarvis.h \
    jarvisServer/sjarvisnodeserver.h \
    jarvisServer/sjarvistcpserver.h \
    jarvisServer/gjarvisserverwidget.h \
    jarvisServer/gnodesimplewidget.h \
    jarvisServer/qconnectdialog.h \
    common/jarvisnodetestapp.h

FORMS   += \
    common/gblinkwidget.ui \
    common/gnodecomponentwidget.ui \
    common/gnodetestwidget.ui \
    common/gsimplegraph.ui \
    common/gwidgetnode.ui \
    common/qimageselectionwidget.ui \
    jarvisServer/gjarvisgui.ui \
    jarvisServer/gjarvisserverwidget.ui \
    jarvisServer/gnodesimplewidget.ui \
    jarvisServer/qconnectdialog.ui \
    common/jarvisnodetestapp.ui

RESOURCES += \
    common/resources.qrc
