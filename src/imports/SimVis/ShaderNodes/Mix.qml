import SimVis 1.0

ShaderNode {
    property var value1
    property var value2
    property var mix

    name: "mix"

    depends: [value1, value2, mix]
    type: value1.type
    result: value1.identifier + " * (1.0 - " + mix.convert("float") + ")" +
            " + " + value2.identifier + " * " + mix.convert("float")
}
