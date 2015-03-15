LIBS += -L../../src -lSimVis
INCLUDEPATH += ../../src
QML_IMPORT_PATH += ../../src

macx {
    LIB_TARGET_DIR = $$OUT_PWD/$${TARGET}.app/Contents/MacOS
    LIB_FILE_NAME = libSimVis.1.0.0.dylib
    LIB_TARGET_FILE = $$LIB_TARGET_DIR/$$LIB_FILE_NAME
    copydata.commands = $$QMAKE_MKDIR $$LIB_TARGET_DIR && $(COPY_DIR) ../../src/SimVis $$LIB_TARGET_DIR
    copydata.commands += && install_name_tool -change libSimVis.1.dylib @executable_path/SimVis/$$LIB_FILE_NAME $$OUT_PWD/$${TARGET}.app/Contents/MacOS/$${TARGET}
    first.depends = $(first) copydata
    export(first.depends)
    export(copydata.commands)
    QMAKE_EXTRA_TARGETS += first copydata
}
