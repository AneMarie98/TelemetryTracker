TEMPLATE = app
INCLUDEPATH += .

QT+= core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QT+= charts

CONFIG += c++17

QT += charts

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    CONTROLLER/fileController.cpp \
    CONTROLLER/jsonController.cpp \
    SENSOR/abstractSensor.cpp \
    SENSOR/brakeTemperatureSensor.cpp \
    SENSOR/fuelFlowSensor.cpp \
    SENSOR/tirePressureSensor.cpp \
    VIEW/sensorWidget.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    CONTROLLER/fileController.h \
    CONTROLLER/jsonController.h \
    SENSOR/abstractSensor.h \
    SENSOR/brakeTemperatureSensor.h \
    SENSOR/fuelFlowSensor.h \
    SENSOR/sensorVisitor.h \
    SENSOR/tirePressureSensor.h \
    VIEW/sensorWidget.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

RESOURCES += \
    assets.qrc



# Default rules for deployment.
#qnx: target.path = /tmp/$${TARGET}/bin
#else: unix:!android: target.path = /opt/$${TARGET}/bin
#!isEmpty(target.path): INSTALLS += target
