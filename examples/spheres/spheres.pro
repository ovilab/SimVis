TEMPLATE = app
CONFIG += c++11

QT += qml quick widgets opengl openglextensions
QT += qml quick 3dcore 3drender 3dinput 3dquick

SOURCES += \
    spheressimulator.cpp \
    main_spheres.cpp \
    instancebuffer.cpp \
    pointgeometry.cpp

RESOURCES += \
    spheres_qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(../examples_vendor.pri)

HEADERS += \
    spheressimulator.h \
    instancebuffer.h \
    pointgeometry.h

DISTFILES += \
    regular.frag \
    regular.vert
