#-------------------------------------------------
#
# Project created by QtCreator 2015-11-27T01:26:53
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = JarvisTestAppAndroid
TEMPLATE = app

INCLUDEPATH+= \
    ../JarvisFirmware/ \
    common/ \
    testAppAndroid/



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
    testAppAndroid/main.cpp \
    jarvisnodetestapp.cpp


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
    jarvisnodetestapp.h


FORMS   += \
    common/gblinkwidget.ui \
    common/gnodecomponentwidget.ui \
    common/gnodetestwidget.ui \
    common/gsimplegraph.ui \
    common/gwidgetnode.ui \
    common/qimageselectionwidget.ui \
    jarvisnodetestapp.ui

DISTFILES +=

RESOURCES += \
    common/resources.qrc

