import QtQuick 2.0

Value {
    property Value value

    name: "sine"

    depends: [value]
    type: value.type
    result: "sin(" + value.identifier + ")"
}
