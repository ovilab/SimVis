import QtQuick 2.0

Value {
    property Value value1
    property Value value2
    property Value mix

    name: "mix"

    depends: [value1, value2, mix]
    type: value1.type
    result: value1.identifier + " * (1.0 - " + mix.convert("float") + ")" +
            " + " + value2.identifier + " * " + mix.convert("float")
}
