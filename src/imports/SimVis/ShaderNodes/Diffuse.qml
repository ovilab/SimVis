import QtQuick 2.0

Value {
    property Value color
    property Value normal
    property Float roughness

    identifier: "diffuse"
    depends: [color, normal]
    type: color.type
    result: color.name // + " + " + normal.name
}
