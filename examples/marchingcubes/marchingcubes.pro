TEMPLATE = app
CONFIG += c++11

QT += qml quick widgets opengl openglextensions

SOURCES += \
    main.cpp \
    mysimulator.cpp \
    perlinnoise.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(../examples_vendor.pri)

HEADERS += \
    mysimulator.h \
    perlinnoise.h
