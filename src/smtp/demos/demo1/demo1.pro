 #-------------------------------------------------
#
# Project created by QtCreator 2014-10-30T22:19:03
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = demo1
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += \
    demo1.cpp

# Location of SMTP Library
SMTP_LIBRARY_LOCATION = /home/denis/src/git/salon/src/smtp

win32:CONFIG(release, debug|release): LIBS += -L$$SMTP_LIBRARY_LOCATION/release/ -L/home/denis/src/git/salon/src/smtp/demos/demo1 -lSMTPEmail
else:win32:CONFIG(debug, debug|release): LIBS += -L$$SMTP_LIBRARY_LOCATION/debug/ -L/home/denis/src/git/salon/src/smtp/demos/demo1 -lSMTPEmail
else:unix: LIBS += -L/home/denis/src/git/salon/src/smtp/demos/demo1 -lSMTPEmail

INCLUDEPATH += $$SMTP_LIBRARY_LOCATION
DEPENDPATH += /home/denis/src/git/salon/src/smtp/demos/demo1
