TEMPLATE = lib
TARGET = SimVis
CONFIG += qt plugin c++11

message(SimVis src simvis_srcdir: $$simvis_srcdir)
message(SimVis src simvis_builddir: $$simvis_builddir)

ios {
    CONFIG += static
    DEFINES += STATIC_BUILD
    QMAKE_MOC_OPTIONS += -Muri=SimVis
}

TARGET = $$qtLibraryTarget($$TARGET)
uri = com.mycompany.qmlcomponents

QT += qml quick widgets opengl openglextensions

include(../package.pri)

DESTDIR = $$simvis_builddir/dist/SimVis
HEADERS_DESTDIR = $$simvis_builddir/dist/include

# Used for dynamic builds

!equals(_PRO_FILE_PWD_, $$OUT_PWD) {
    copy_qmldir.target = $$DESTDIR/qmldir
    copy_qmldir.depends = $$_PRO_FILE_PWD_/imports/SimVis/qmldir
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

DISTFILES +=
