import QtQuick 2.0

Value {
    property real value
    header: "uniform " + type + " $in;\n"
    identifier: "float"
    type: "float"
    result: "$in"
}
