import QtQuick 2.0

Value {
    property Value value1
    property Value value2

    identifier: "add"

    depends: [value1, value2]
    type: value1.type
    result: value1.name + " + " + value2.name
}
