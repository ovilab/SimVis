import QtQuick 2.0
import SimVis 1.0

Value {
    isUniform: true
    header: "uniform " + type + " " + identifier + ";\n"
    name: "univalue"
    GlslVariantBridge {
        id: bridge
    }
    type: bridge.type(uniformValue)

    result: ""
}
