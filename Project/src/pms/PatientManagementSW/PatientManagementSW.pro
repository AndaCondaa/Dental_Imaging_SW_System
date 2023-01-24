QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    enrollmanager.cpp \
    imagemanager.cpp \
    main.cpp \
    mainwindow.cpp \
    medicalchart.cpp \
    medicalrecordmanager.cpp \
    networkmanager.cpp \
    patientinfomanager.cpp \
    patientstatusmanager.cpp

HEADERS += \
    enrollmanager.h \
    imagemanager.h \
    mainwindow.h \
    medicalchart.h \
    medicalrecordmanager.h \
    networkmanager.h \
    patientinfomanager.h \
    patientstatusmanager.h

FORMS += \
    enrollmanager.ui \
    imagemanager.ui \
    mainwindow.ui \
    medicalchart.ui \
    medicalrecordmanager.ui \
    patientinfomanager.ui \
    patientstatusmanager.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
