QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    audio_system.cpp \
    building.cpp \
    control_system.cpp \
    display_system.cpp \
    elevator_car.cpp \
    elevator_doors.cpp \
    elevator_doors_sensors.cpp \
    floor_control_panel.cpp \
    floor_sensor.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    audio_system.h \
    building.h \
    control_system.h \
    defs.h \
    display_system.h \
    elevator_car.h \
    elevator_doors.h \
    elevator_doors_sensors.h \
    floor_control_panel.h \
    floor_sensor.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    COMP3004-ElevatorSimulation.pro.user
