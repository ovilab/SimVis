LIB_NAME = SimVis
LIBS += -L$$OUT_PWD/../../dist/$${LIB_NAME} -l$${LIB_NAME}
INCLUDEPATH += $$_PRO_FILE_PWD_/../../src

macx {
    copy_lib.commands = $(COPY_DIR) $$OUT_PWD/../../dist/$${LIB_NAME} $$OUT_PWD/Contents/MacOS
    copydata.commands += && install_name_tool -change lib$${LIB_NAME}.dylib @executable_path/$${LIB_NAME}/lib$${LIB_NAME}.dylib $$OUT_PWD/$${TARGET}.app/Contents/MacOS/$${TARGET}
    QMAKE_EXTRA_TARGETS += copy_lib
} unix {
    copy_lib.commands = $(COPY_DIR) $$OUT_PWD/../../dist/$${LIB_NAME} $$OUT_PWD
    QMAKE_EXTRA_TARGETS += copy_lib
}

first.depends = $(first) copy_lib
export(first.depends)
export(copy_lib.commands)
QMAKE_EXTRA_TARGETS += first copy_lib

