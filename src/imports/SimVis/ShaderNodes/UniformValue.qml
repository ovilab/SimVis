import QtQuick 2.0

Value {
    property var value
    isUniform: true
    header: "uniform " + type + " " + name + ";\n"
    identifier: "univalue"
    type: "float"
    result: ""
}
