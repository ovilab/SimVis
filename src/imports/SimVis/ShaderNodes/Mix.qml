import SimVis 1.0

ShaderNode {
    property var value1
    property var value2
    property var mix: 0.5

    VariantShaderNode {
        id: _mix
        value: mix
    }
    VariantShaderNode {
        id: _value1
        value: value1
    }
    VariantShaderNode {
        id: _value2
        value: value2
    }

    name: "mix"
    type: value1.type
    result: resolve(_value1) + " * (1.0 - " + resolve(_mix, "float") + ")" +
            " + " + resolve(_value2) + " * " + resolve(_mix, "float")

}
