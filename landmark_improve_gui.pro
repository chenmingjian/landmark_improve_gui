#-------------------------------------------------
#
# Project created by QtCreator 2019-01-27T14:25:08
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = landmark_improve_gui
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.

DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += chen

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    tool.cpp \
    imglabel.cpp \
    namelistwidget.cpp \
    selectitem.cpp \
    opentask.cpp \
    newtask.cpp \
    tasklist.cpp

HEADERS += \
        mainwindow.h \
    tool.h \
    imglabel.h \
    namelistwidget.h \
    selectitem.h \
    opentask.h \
    newtask.h \
    tasklist.h \
    pixandcv.h

FORMS += \
        mainwindow.ui \
    opentask.ui \
    newtask.ui \
    newtask.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=

RESOURCES += \
    img.qrc

RC_ICONS = logo.ico

INCLUDEPATH += D:\Program_Files\opencv4.1.1\build\include

LIBS += D:\Program_Files\opencv4.1.1\build\x64\vc15\lib\opencv_world411.lib

