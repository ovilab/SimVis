TEMPLATE = lib
TARGET = SimVis
CONFIG += qt plugin c++11

TARGET = $$qtLibraryTarget($$TARGET)
uri = com.mycompany.qmlcomponents

QT += qml quick widgets opengl openglextensions

SOURCES += \
    simulator.cpp \
    visualizer.cpp \
    renderable.cpp \
    billboards2d.cpp \
    points2d.cpp \
    camera.cpp \
    quickworker.cpp \
    navigator.cpp \
    simvisplugin.cpp

HEADERS += \
    simulator.h \
    visualizer.h \
    renderable.h \
    billboards2d.h \
    points2d.h \
    camera.h \
    quickworker.h \
    navigator.h \
    simvisplugin.h

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
