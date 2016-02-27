import QtQuick 2.0

Value {
    property real value
    header: "uniform " + type + " $in;\n"
    identifiertifier: "float"
    type: "float"
    result: "$in"
}
