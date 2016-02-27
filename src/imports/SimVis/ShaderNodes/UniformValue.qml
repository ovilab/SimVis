import QtQuick 2.0
import SimVis 1.0

Value {
    property var value
    isUniform: true
    header: "uniform " + type + " " + name + ";\n"
    identifier: "univalue"
    GlslVariantBridge {
        id: bridge
    }
    type: bridge.type(value)

    result: ""
}
