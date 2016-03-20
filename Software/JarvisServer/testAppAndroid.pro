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
    testAppAndroid/main.cpp \
    common/jarvisnodetestapp.cpp \
    common/gnodeconfigdialog.cpp \
    common/qwificonfigdialog.cpp \
    common/grtttlplayer.cpp


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
    common/jarvisnodetestapp.h \
    common/gnodeconfigdialog.h \
    common/qwificonfigdialog.h \
    common/grtttlplayer.h \
    ../JarvisFirmware/settings.h


FORMS   += \
    common/gblinkwidget.ui \
    common/gnodecomponentwidget.ui \
    common/gnodetestwidget.ui \
    common/gsimplegraph.ui \
    common/gwidgetnode.ui \
    common/qimageselectionwidget.ui \
    common/jarvisnodetestapp.ui \
    common/gnodeconfigdialog.ui \
    common/qwificonfigdialog.ui \
    common/grtttlplayer.ui

DISTFILES += \
    testAppAndroid/android/AndroidManifest.xml \
    testAppAndroid/android/gradle/wrapper/gradle-wrapper.jar \
    testAppAndroid/android/gradlew \
    testAppAndroid/android/res/values/libs.xml \
    testAppAndroid/android/build.gradle \
    testAppAndroid/android/gradle/wrapper/gradle-wrapper.properties \
    testAppAndroid/android/gradlew.bat

RESOURCES += \
    common/resources.qrc

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/testAppAndroid/android

