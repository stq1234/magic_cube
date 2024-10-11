#-------------------------------------------------
#
# Project created by QtCreator 2023-08-18T19:05:26
#
#-------------------------------------------------

QT       += core gui
QT       += opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = magic_cube
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        widget.cpp \
        solve.cpp \
        scan.cpp \
        state.cpp \
        globel.cpp \
    show.cpp

HEADERS += \
        widget.h \
        solve.h \
        scan.h \
        state.h \
        globel.h \
    show.h

FORMS += \
        widget.ui \
        solve.ui \
        scan.ui \
    show.ui

win32:LIBS += -lOpengl32\
-lglu32
unix:LIBS += -lglut -lGLU

INCLUDEPATH += C:\Qt\opencv\opencv_build\install\include

LIBS += C:\Qt\opencv\opencv_build\lib\libopencv_*.a

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
