#-------------------------------------------------
#
# Project created by QtCreator 2014-05-20T03:49:55
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT += network

TARGET = OpenItemSets
TEMPLATE = app

#Application version
VERSION_MAJOR = 0
VERSION_MINOR = 0
VERSION_BUILD = 1

DEFINES += "VERSION_MAJOR=$$VERSION_MAJOR"\
       "VERSION_MINOR=$$VERSION_MINOR"\
       "VERSION_BUILD=$$VERSION_BUILD"

#Target version
VERSION = $${VERSION_MAJOR}.$${VERSION_MINOR}.$${VERSION_BUILD}


SOURCES += src/main.cpp \
    src/mainwindow.cpp \
    src/settingsdialog.cpp \
    src/settings.cpp \
    src/leaguedirectory.cpp \
    src/apicall.cpp

HEADERS  +=  src/mainwindow.h \
    src/settings.h \
    src/settingsdialog.h \
    src/leaguedirectory.h \
    src/apicall.h

FORMS    += src/mainwindow.ui \
    src/settingsdialog.ui
