import SimVis 1.0

ShaderNode {
    property ShaderNode value1
    property ShaderNode value2

    name: "add"

    depends: [value1, value2]
    type: value1.type
    result: value1.identifier + " + " + value2.identifier
}
