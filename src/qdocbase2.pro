# ****************************************************************
#
# Do not change the code below if it is not very necessary!
#
# ****************************************************************
QT += core sql network xml concurrent widgets printsupport gui

win32: {
QT += winextras
}

CONFIG += release

# --------------------------------------------------
message(This program uses C++11 features)
CONFIG += c++11
# --------------------------------------------------

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# IMPORTANT
# ---------
# - The lines below are responsible
# - We use as the version identifier the month and year of the changes,
#   for example: April'2020 for get the current version of the program
# - Set the path below for your installation, changing the original path to your project directory.
# - If you change the 'git tag' you must recompile the qdocbase2 project so that the new tag is shown in the about window.
unix: {
GIT_VERSION = $$system(git --git-dir=/dados/desenvolvimento/qt5-apps/qDocBase2/.git --work-tree=/dados/desenvolvimento/qt5-apps/qDocBase2 describe --always --tags)
GVERSION = \\\"'$${GIT_VERSION}'\\\"
DEFINES += "VERSION_NUMBER=$${GVERSION}"
}
win32: {
GIT_VERSION = $$system(git describe --always --tags)
GVERSION = \\\"'$${GIT_VERSION}'\\\"
DEFINES += "VERSION_NUMBER=$${GVERSION}"
}

# Official name of the program
PROGNAME  = \\\"qDocBase2\\\"
DEFINES += "PROGRAM_NAME=$${PROGNAME}"

#TRANSLATIONS = ../translations/en_US.ts

CONFIG += warn_on thread qt
SOURCES += qdocbase.cpp \
           main.cpp \
 lvmodulosp.cpp \
 configuracao.cpp \
 vabstractprint.cpp \
    recorddocs.cpp \
    tventity.cpp \
    typeimage.cpp \
    qvpmultifiles.cpp \
    utilities.cpp \
    tvfinddocs.cpp \
    vcplib/cnpj.cpp \
    vcplib/cpf.cpp \
    vcplib/vcpdoublevalidator.cpp \
    vcplib/vcpftp.cpp \
    vcplib/vcplib2.cpp \
    vcplib/vcplowervalidator.cpp \
    vcplib/vcpmdiarea.cpp \
    vcplib/vcpuppervalidator.cpp \
    vstandarditemmodel_p.cpp \
    recordentity.cpp \
    dialogconfig.cpp \
    topsecret.cpp \
    dialogabout.cpp \
    printimage.cpp \
    printentity.cpp \
    printdocgeneral.cpp \
    printdocanalitic.cpp \
    printselectedimages.cpp \
    department.cpp \
    advsearch.cpp \
    advsearchtableviewitemdelegate.cpp \
    temporaryfiles.cpp \
    printselectedimages2.cpp \
    sysactivity.cpp

HEADERS += qdocbase.h \
 lvmodulosp.h \
 configuracao.h \
 vabstractprint.h \
    recorddocs.h \
    tventity.h \
    typeimage.h \
    qvpmultifiles.h \
    utilities.h \
    tvfinddocs.h \
    vcplib/cnpj.h \
    vcplib/cpf.h \
    vcplib/vcpdoublevalidator.h \
    vcplib/vcpftp.h \
    vcplib/vcplib2.h \
    vcplib/vcplowervalidator.h \
    vcplib/vcpmdiarea.h \
    vcplib/vcpuppervalidator.h \
    vstandarditemmodel_p.h \
    recordentity.h \
    dialogconfig.h \
    topsecret.h \
    dialogabout.h \
    printimage.h \
    printentity.h \
    printdocgeneral.h \
    printdocanalitic.h \
    printselectedimages.h \
    department.h \
    advsearch.h \
    advsearchtableviewitemdelegate.h \
    temporaryfiles.h \
    printselectedimages2.h \
    sysactivity.h

TEMPLATE = app
TARGET = qdocbase2
DESTDIR = ../bin

RESOURCES = application.qrc \
    fileicons.qrc

FORMS += formDocBase.ui \
    formRecordDocs.ui \
    formRecordEntity.ui \
    dialogconfig.ui \
    formTopSec.ui \
    dialogabout.ui \
    formadvsearch.ui

OTHER_FILES += \
    changelog.txt

DISTFILES += \
    ../README.md \
    docbase_struct.sql
