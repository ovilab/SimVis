TEMPLATE = app
CONFIG += c++11

QT += widgets opengl openglextensions qml quick 3dcore 3drender 3dinput 3dquick

SOURCES += \
    main_shadernodes.cpp \
    spheregeometrynew.cpp

RESOURCES += \
    shadernodes_qml.qrc \
    shadernodes_images.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(../examples_vendor.pri)

HEADERS += \
    spheregeometrynew.h \
    spheregeometrynew_p.h
