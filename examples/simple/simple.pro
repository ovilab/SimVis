TEMPLATE = app
CONFIG +=  #c++11

QMAKE_CXXFLAGS += -std=gnu++11 -Wno-braced-scalar-init -Wno-inconsistent-missing-override
INCLUDEPATH += /home/svenni/apps/nacl/nacl_sdk/pepper_47/include
INCLUDEPATH += $$simvis_srcdir/src/

QT += qml quick widgets opengl openglextensions

SOURCES += \
    main.cpp \
    simplesimulator.cpp

RESOURCES += qml.qrc \
    ../../src/shaders.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)
#include(../../library_deployment.pri)

HEADERS += \
    simplesimulator.h

SOURCES += \
    ../../src/core/camera.cpp \
    ../../src/core/navigator.cpp \
    ../../src/core/quickworker.cpp \
    ../../src/core/renderable.cpp \
    ../../src/core/simulator.cpp \
    ../../src/navigators/trackballnavigator.cpp \
    ../../src/core/visualizer.cpp \
    ../../src/core/cameralens.cpp \
    ../../src/core/lookattransform.cpp \
    ../../src/renderables/billboards/billboards.cpp \
    ../../src/renderables/points/points.cpp \
    ../../src/renderables/marchingcubes/marchingcubes.cpp \
    ../../src/renderables/marchingcubes/marchingcubesgenerator.cpp \
    ../../src/core/shadereffect.cpp \
    ../../src/shadereffects/simplexbump.cpp \
    ../../src/shadereffects/simplextexture.cpp \
    ../../src/renderables/spheres/spheres.cpp \
    ../../src/shadereffects/light.cpp \
    ../../src/renderables/skybox/skybox.cpp \
    ../../src/shadereffects/skyboxreflection.cpp \
    ../../src/renderables/trianglecollection/trianglecollection.cpp \
    ../../src/shadereffects/periodiccopies.cpp \
    ../../src/shadereffects/slice.cpp \
    ../../src/renderables/cylinders/cylinders.cpp \
    ../../src/renderables/lines/lines.cpp

HEADERS += \
    ../../src/core/navigator.h \
    ../../src/core/quickworker.h \
    ../../src/core/renderable.h \
    ../../src/core/simulator.h \
    ../../src/core/camera.h \
    ../../src/navigators/trackballnavigator.h \
    ../../src/core/visualizer.h \
    ../../src/core/camera_p.h \
    ../../src/core/cameralens.h \
    ../../src/core/cameralens_p.h \
    ../../src/core/lookattransform.h \
    ../../src/core/lookattransform_p.h \
    ../../src/renderables/billboards/billboards.h \
    ../../src/renderables/points/points.h \
    ../../src/renderables/marchingcubes/marchingcubes.h \
    ../../src/renderables/marchingcubes/marchingcubesgenerator.h \
    ../../src/SimVis/Billboards \
    ../../src/SimVis/Points \
    ../../src/SimVis/MarchingCubes \
    ../../src/core/shadereffect.h \
    ../../src/shadereffects/simplexbump.h \
    ../../src/shadereffects/simplextexture.h \
    ../../src/renderables/spheres/spheres.h \
    ../../src/SimVis/Spheres \
    ../../src/shadereffects/light.h \
    ../../src/renderables/skybox/skybox.h \
    ../../src/shadereffects/skyboxreflection.h \
    ../../src/renderables/trianglecollection/trianglecollection.h \
    ../../src/SimVis/QuickWorker \
    ../../src/SimVis/Renderable \
    ../../src/SimVis/Simulator \
    ../../src/SimVis/TriangleCollection \
    ../../src/shadereffects/periodiccopies.h \
    ../../src/shadereffects/slice.h \
    ../../src/renderables/cylinders/cylinders.h \
    ../../src/SimVis/Cylinders \
    ../../src/renderables/lines/lines.h \
    ../../src/SimVis/Lines
