import QtQuick 2.0

Value {
    property Value value1
    property Value value2

    name: "add"

    depends: [value1, value2]
    type: value1.type
    result: value1.identifier + " + " + value2.identifier
}
