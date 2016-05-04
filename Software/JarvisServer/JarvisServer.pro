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
    jarvisServer/gjarvisgui.cpp \
    jarvisServer/main.cpp \
    jarvisServer/sjarvis.cpp \
    jarvisServer/sjarvisnodeserver.cpp \
    jarvisServer/gjarvisserverwidget.cpp \
    jarvisServer/gnodesimplewidget.cpp \
    jarvisServer/qconnectdialog.cpp \
    common/jarvisnodetestapp.cpp \
    jarvisServer/sjarvisconnection.cpp \
    jarvisServer/sjarvisconnectionmanager.cpp \
    jarvisServer/gjarvisconnectionmanagerwidget.cpp \
    jarvisServer/gjarvisconnectionwidget.cpp \
    common/grtttlplayer.cpp \
    common/qwificonfigdialog.cpp \
    common/gnodeconfigdialog.cpp \
    jarvisServer/gruleeditorwidget.cpp

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
    jarvisServer/gjarvisgui.h \
    jarvisServer/sjarvis.h \
    jarvisServer/sjarvisnodeserver.h \
    jarvisServer/gjarvisserverwidget.h \
    jarvisServer/gnodesimplewidget.h \
    jarvisServer/qconnectdialog.h \
    common/jarvisnodetestapp.h \
    jarvisServer/sjarvisconnection.h \
    jarvisServer/sjarvisconnectionmanager.h \
    jarvisServer/gjarvisconnectionmanagerwidget.h \
    jarvisServer/gjarvisconnectionwidget.h \
    common/grtttlplayer.h \
    common/qwificonfigdialog.h \
    ../JarvisFirmware/settings.h \
    common/gnodeconfigdialog.h \
    jarvisServer/gruleeditorwidget.h

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
    common/jarvisnodetestapp.ui \
    jarvisServer/gjarvisconnectionmanagerwidget.ui \
    jarvisServer/gjarvisconnectionwidget.ui \
    common/grtttlplayer.ui \
    common/qwificonfigdialog.ui \
    common/gnodeconfigdialog.ui \
    jarvisServer/gruleeditorwidget.ui

RESOURCES += \
    common/resources.qrc
