TEMPLATE = app

QT += qml quick widgets opengl openglextensions
CONFIG += c++11

SOURCES += main.cpp \
    bondssimulator.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)
include(../../library_deployment.pri)

HEADERS += \
    bondssimulator.h
