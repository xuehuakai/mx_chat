QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    clicklabel.cpp \
    global.cpp \
    httpmgr.cpp \
    logindialog.cpp \
    main.cpp \
    mainwindow.cpp \
    regusterdialog.cpp \
    resetdialog.cpp \
    timberbtn.cpp

HEADERS += \
    clicklabel.h \
    global.h \
    httpmgr.h \
    logindialog.h \
    mainwindow.h \
    regusterdialog.h \
    resetdialog.h \
    singleton.h \
    timberbtn.h

FORMS += \
    logindialog.ui \
    mainwindow.ui \
    regusterdialog.ui \
    resetdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    rc.qrc

DISTFILES += \
    config.ini
