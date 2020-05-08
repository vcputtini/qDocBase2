QT += core sql network xml concurrent widgets printsupport gui

CONFIG += release

# --------------------------------------------------
message(This program uses C++11 features)
CONFIG += c++11
# --------------------------------------------------

message(vcplib2: This library is required for the application to work properly.)
win32: {
    QT += winextras
    LIBS += -lmysql -lvcplib22
    CONFIG += release
# CONFIG += console
}
unix : LIBS += -lvcplib2


# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

#The lines below are responsible for get the current version of the program
# Note: Set the path below for your installation
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
PROGNAME  = \\\"DocBase\\\"
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
    changelog.txt \
    estrutura_tabelas.sql

DISTFILES += \
    ../README.md \
    docbase_struct.sql
