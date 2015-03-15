LIB_NAME = SimVis
LIB_TARGET = $$qtLibraryTarget($$LIB_NAME)
LIBS += -L$$OUT_PWD/../../dist/$${LIB_NAME} -l$${LIB_TARGET}
INCLUDEPATH += $$_PRO_FILE_PWD_/../../src

macx {
    copy_lib.commands = $(COPY_DIR) $$OUT_PWD/../../dist/$${LIB_NAME} $$OUT_PWD/$${TARGET}.app/Contents/MacOS
    copy_lib.commands += && install_name_tool -change lib$${LIB_TARGET}.dylib @executable_path/$${LIB_NAME}/lib$${LIB_TARGET}.dylib $$OUT_PWD/$${TARGET}.app/Contents/MacOS/$${TARGET}
} unix:!macx {
    copy_lib.commands = $(COPY_DIR) $$OUT_PWD/../../dist/$${LIB_NAME} $$OUT_PWD
}

first.depends = $(first) copy_lib
export(first.depends)
export(copy_lib.commands)
QMAKE_EXTRA_TARGETS += first copy_lib

