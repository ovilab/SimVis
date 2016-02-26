import QtQuick 2.0

Value {
    property Value value

    identifier: "sine"

    depends: [value]
    type: value.type
    result: "sin(" + value.name + ")"
}
