import SimVis 1.0

ShaderNode {
    property ShaderNode value

    name: "sine"

    depends: [value]
    type: value.type
    result: "sin(" + value.identifier + ")"
}
