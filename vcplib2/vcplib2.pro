#-------------------------------------------------
#
# Project created by QtCreator 2013-05-13T17:28:16
#
#-------------------------------------------------


# --------------------------------------------------
message(We will be using CPP11 features)
CONFIG += c++11
# --------------------------------------------------

QT       += widgets network sql xml printsupport

TARGET = vcplib2
TEMPLATE = lib
VERSION = 2

DEFINES += VCPLIB2_LIBRARY

SOURCES += vcplib2.cpp \
    vcpmdiarea.cpp \
    vcpuppervalidator.cpp \
    vcplowervalidator.cpp \
    vcpdoublevalidator.cpp \
    vcpftp.cpp \
    cnpj.cpp \
    cpf.cpp

HEADERS += vcplib2.h\
        vcplib2_global.h \
    vcpmdiarea.h \
    vcpuppervalidator.h \
    vcplowervalidator.h \
    vcpdoublevalidator.h \
    vcpftp.h \
    cnpj.h \
    cpf.h

OTHER_FILES += \
    README.txt

DISTFILES += \
    changelog.txt
