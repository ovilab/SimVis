import QtQuick 2.0

Value {
    property Value value1
    property Value value2
    property Value mix

    identifier: "mix"

    depends: [value1, value2, mix]
    type: value1.type
    result: value1.name + " * (1.0 - " + mix.convert("float") + ")" +
            " + " + value2.name + " * " + mix.convert("float")
}
