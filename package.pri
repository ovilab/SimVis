message(Library Deployment)
message(pwd: $$PWD)
message(shadowed_pwd: $$shadowed($$PWD))
message(out_pwd: $$OUT_PWD)

contains(TEMPLATE, subdirs) {
    SUBDIRS += src
    src.subdir = $$PWD/src
} else {
    LIB_NAME = SimVis

    LIB_TARGET = $$qtLibraryTarget($$LIB_NAME)
    LIBS += -L$$shadowed($$PWD)/dist/$${LIB_NAME} -l$${LIB_TARGET}

#    QML_IMPORT_PATH += $$shadowed($$PWD)
#    QML2_IMPORT_PATH += $$QML_IMPORT_PATH
    INCLUDEPATH += $$PWD/src/

    # Deployment of library
    ios {
        simvis_lib.files = $$shadowed($$PWD)/src/imports/SimVis
        simvis_lib.path = qt_qml
        QMAKE_BUNDLE_DATA += simvis_lib
        # undocumented Qmake property used to autogenerate Q_IMPORT_PLUGIN
        QMLPATHS += $$shadowed($$PWD)/src/imports
    } macx {
        copy_lib.commands = $(COPY_DIR) $$shadowed($$PWD)/dist/$${LIB_NAME} $$OUT_PWD/$${TARGET}.app/Contents/MacOS
        copy_lib.commands += && install_name_tool -change lib$${LIB_TARGET}.dylib @executable_path/$${LIB_NAME}/lib$${LIB_TARGET}.dylib $$OUT_PWD/$${TARGET}.app/Contents/MacOS/$${TARGET}
    } unix:!macx {
        copy_lib.commands = $(COPY_DIR) $$shadowed($$PWD)/dist/$${LIB_NAME} $$OUT_PWD
    }

    first.depends = $(first) copy_lib
    export(first.depends)
    export(copy_lib.commands)
    QMAKE_EXTRA_TARGETS += first copy_lib
}
