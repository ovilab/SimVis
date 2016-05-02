TEMPLATE = lib
TARGET = SimVis
CONFIG += qt plugin c++11

message(SimVis src simvis_srcdir: $$simvis_srcdir)
message(SimVis src simvis_builddir: $$simvis_builddir)

QT += widgets opengl openglextensions qml quick 3dcore 3drender 3dinput 3dquick

ios {
    CONFIG += static
    DEFINES += STATIC_BUILD
    QMAKE_MOC_OPTIONS += -Muri=SimVis
}

TARGET = $$qtLibraryTarget($$TARGET)
uri = com.mycompany.qmlcomponents

include(../package.pri)

DISTFILES = qmldir \
    core/simvisplugin.json \
    renderables/spheres/spheres_node.vsh \
    renderables/spheres/spheres_node_geom.vsh \
    shadereffects/shaders/defaultvertex.glsl \
    shadereffects/shaders/defaultgeometry.glsl


