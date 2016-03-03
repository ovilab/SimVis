TEMPLATE = app
CONFIG += c++11

QT += qml quick widgets opengl openglextensions

SOURCES += \
    qmatplotppsimulator.cpp \
    main_qmatplotpp.cpp

RESOURCES += \
    qmatplotpp_qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(../examples_vendor.pri)

HEADERS += \
    qmatrplotppsimulator.h
