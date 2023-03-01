QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    apihandle.cpp \
    iscandevice.cpp \
    machine.cpp \
    main.cpp \
    mainwindow.cpp \
    rfid.cpp \
    scanscreen.cpp \
    sl500.cpp \
    standbyscreen.cpp

HEADERS += \
    apihandle.h \
    iscandevice.h \
    machine.h \
    mainwindow.h \
    rfid.h \
    scanscreen.h \
    sl500.h \
    standbyscreen.h

FORMS += \
    mainwindow.ui \
    scanscreen.ui \
    standbyscreen.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
