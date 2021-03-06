#-------------------------------------------------
#
# Project created by QtCreator 2018-04-15T09:35:36
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ScriptSDCC
TEMPLATE = app


SOURCES += main.cpp\
    eden.cpp \
    qmouselabel.cpp \
    qplaintexteditkey.cpp \
    mainwindow.cpp \
    membuffer.cpp \
    graphfont.cpp \
    appcore.cpp \
    scriptmachine.cpp \
    scriptmachinemcs51.cpp \
    scriptmachinez180.cpp \
    projectitem.cpp \
    ioconsolewin.cpp \
    ioconsole.cpp \
    iospreadsheet.cpp \
    iospreadsheetwin.cpp \
    iographwin.cpp \
    iograph.cpp \
    configfile.cpp \
    filehandle.cpp

HEADERS  += mainwindow.h \
    eden.h \
    binary.h \
    objmem.h \
    qmouselabel.h \
    qplaintexteditkey.h \
    membuffer.h \
    graphfont.h \
    appcore.h \
    scriptmachine.h \
    scriptmachinemcs51.h \
    scriptmachinez180.h \
    projectitem.h \
    ioconsolewin.h \
    ioconsole.h \
    iospreadsheet.h \
    iospreadsheetwin.h \
    iographwin.h \
    iograph.h \
    configfile.h \
    filehandle.h

FORMS    += mainwindow.ui \
    ioconsolewin.ui \
    iospreadsheetwin.ui \
    iographwin.ui

CONFIG   += c++11
