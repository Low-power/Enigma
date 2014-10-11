#-------------------------------------------------
#
# Project created by QtCreator 2014-03-22T14:15:37
#
#-------------------------------------------------

QT       += core gui
QT	 += network

TARGET = Server
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    settingdialog.cpp \
    aboutdialog.cpp \
    data.cpp \
    server.cpp \
    model.cpp \
    querydialog.cpp \
    buildsvrdialog.cpp \
    upload.cpp \
    download.cpp

HEADERS  += mainwindow.h \
    settingdialog.h \
    aboutdialog.h \
    common.h \
    server.h \
    model.h \
    querydialog.h \
    buildsvrdialog.h \
    upload.h \
    download.h

FORMS    += mainwindow.ui \
    settingdialog.ui \
    aboutdialog.ui \
    querydialog.ui \
    buildsvrdialog.ui

RESOURCES += \
    icons.qrc

CCFLAG += -Wno-struct

win32: RC_FILE +=  app.rc
