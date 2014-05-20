#-------------------------------------------------
#
# Project created by QtCreator 2014-05-20T03:49:55
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OpenItemSets
TEMPLATE = app


SOURCES += src/main.cpp \
    src/mainwindow.cpp \
    src/settingsdialog.cpp \
    src/settings.cpp

HEADERS  +=  src/mainwindow.h \
    src/settings.h \
    src/settingsdialog.h

FORMS    += src/mainwindow.ui \
    src/settingsdialog.ui
