
QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    imagealbum.cpp \
    imagescene.cpp \
    imageview.cpp \
    loginform.cpp \
    main.cpp \
    mainwindow.cpp \
    networkmanager.cpp \
    patientinfo.cpp \
    prescription.cpp

HEADERS += \
    imagealbum.h \
    imagescene.h \
    imageview.h \
    loginform.h \
    mainwindow.h \
    networkmanager.h \
    patientinfo.h \
    prescription.h

FORMS += \
    imagealbum.ui \
    loginform.ui \
    mainwindow.ui \
    patientinfo.ui \
    prescription.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target




win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../opencv/build/x64/vc15/lib/ -lopencv_world452
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../opencv/build/x64/vc15/lib/ -lopencv_world452d

INCLUDEPATH += $$PWD/../../../../../opencv/build/x64/vc15
DEPENDPATH += $$PWD/../../../../../opencv/build/x64/vc15

INCLUDEPATH += $$PWD/../../../../../opencv/build/include
