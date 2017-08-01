#-------------------------------------------------
#
# Project created by QtCreator 2016-02-16T11:33:54
#
#-------------------------------------------------

QT       += core gui network xml sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qtSerien
TEMPLATE = app


SOURCES += main.cpp\
        cmainwindow.cpp \
    cserie.cpp \
    cseasondelegate.cpp \
    cseason.cpp \
    cepisode.cpp \
    csearch.cpp \
    cmessageanimatedialog.cpp \
    cedit.cpp \
    cimage.cpp \
    cseasondetails.cpp \
    cverticallabel.cpp \
    cepisodedetails.cpp \
    cmessagedialog.cpp \
    cupdatethread.cpp \
    cpicturesthread.cpp \
    cthetvdbv2.cpp \
    cthemoviedbv3.cpp \
    cmovie.cpp

HEADERS  += cmainwindow.h \
    cserie.h \
    cseasondelegate.h \
    cseason.h \
    cepisode.h \
    csearch.h \
    cmessageanimatedialog.h \
    cedit.h \
    cimage.h \
    cseasondetails.h \
    cverticallabel.h \
    cepisodedetails.h \
    cmessagedialog.h \
    cupdatethread.h \
    cpicturesthread.h \
    cthetvdbv2.h \
    cthemoviedbv3.h \
    cmovie.h

FORMS    += cmainwindow.ui \
    csearch.ui \
    cmessageanimatedialog.ui \
    cedit.ui \
    cseasondetails.ui \
    cepisodedetails.ui \
    cmessagedialog.ui

DISTFILES +=

RESOURCES += \
    resource.qrc
