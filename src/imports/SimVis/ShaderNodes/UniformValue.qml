import SimVis 1.0

ShaderNode {
    isUniform: true
    header: "uniform " + type + " " + identifier + ";\n"
    name: "univalue"
    type: glslTypeFromVariant(uniformValue)

    result: ""
}
