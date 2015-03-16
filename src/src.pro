TEMPLATE = lib
TARGET = SimVis
CONFIG += qt plugin c++11

TARGET = $$qtLibraryTarget($$TARGET)
uri = com.mycompany.qmlcomponents

QT += qml quick widgets opengl openglextensions

SOURCES += \
    renderables/billboards.cpp \
    core/camera.cpp \
    core/navigator.cpp \
    renderables/points.cpp \
    core/quickworker.cpp \
    core/renderable.cpp \
    core/simulator.cpp \
    core/simvisplugin.cpp \
    navigators/trackballnavigator.cpp \
    core/visualizer.cpp \
    core/cameralens.cpp \
    core/lookattransform.cpp

HEADERS += \
    core/navigator.h \
    core/quickworker.h \
    core/renderable.h \
    core/simulator.h \
    core/camera.h \
    renderables/billboards.h \
    renderables/points.h \
    core/simvisplugin.h \
    navigators/trackballnavigator.h \
    core/visualizer.h \
    core/camera_p.h \
    core/cameralens.h \
    core/cameralens_p.h \
    core/lookattransform.h \
    core/lookattransform_p.h

DISTFILES = qmldir

DESTDIR = $$OUT_PWD/../dist/SimVis
HEADERS_DESTDIR = $$OUT_PWD/../dist/include

!equals(_PRO_FILE_PWD_, $$OUT_PWD) {
    copy_qmldir.target = $$DESTDIR/qmldir
    copy_qmldir.depends = $$_PRO_FILE_PWD_/qmldir
    copy_qmldir.commands = $(COPY_FILE) \"$$replace(copy_qmldir.depends, /, $$QMAKE_DIR_SEP)\" \"$$replace(copy_qmldir.target, /, $$QMAKE_DIR_SEP)\"
    QMAKE_EXTRA_TARGETS += copy_qmldir
    PRE_TARGETDEPS += $$copy_qmldir.target
}

qmldir.files = qmldir
unix {
    installPath = $$[QT_INSTALL_QML]/$$replace(uri, \\., /)
    qmldir.path = $$installPath
    target.path = $$installPath
    INSTALLS += target qmldir
}
