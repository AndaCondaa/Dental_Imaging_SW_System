QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    controlpanel.cpp \
    imagingmanager.cpp \
    main.cpp \
    mainnetworkmanager.cpp \
    mainwindow.cpp \
    packetdata.cpp \
    patientmanager.cpp \
    protocol.cpp \
    subnetworkmanager.cpp

HEADERS += \
    controlpanel.h \
    imagingmanager.h \
    mainnetworkmanager.h \
    mainwindow.h \
    packetdata.h \
    patientmanager.h \
    protocol.h \
    subnetworkmanager.h

FORMS += \
    controlpanel.ui \
    imagingmanager.ui \
    mainwindow.ui \
    patientmanager.ui


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../opencv/build/x64/vc15/lib/ -lopencv_world452
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../opencv/build/x64/vc15/lib/ -lopencv_world452d

INCLUDEPATH += $$PWD/../../../../../opencv/build/x64/vc15
DEPENDPATH += $$PWD/../../../../../opencv/build/x64/vc15

INCLUDEPATH += $$PWD/../../../../../opencv/build/include

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../../../../opencv/build/x64/vc15/lib/ -lopencv_world452
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../../../../opencv/build/x64/vc15/lib/ -lopencv_world452d

INCLUDEPATH += $$PWD/../../../../../../../../opencv/build/x64/vc15
DEPENDPATH += $$PWD/../../../../../../../../opencv/build/x64/vc15

INCLUDEPATH += $$PWD/../../../../../../../../opencv/build/include
