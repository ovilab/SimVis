import SimVis 1.0

ShaderNode {
    property var color
    property var normal
    property var position
    property ShaderGroup lights: ShaderGroup {
        Light {}
    }

    name: "diffuse"
    // TODO make type general (convert to vec3 or vec4)
    type: "vec3"
    source: {
        var output = ""
        output += "vec3 $diffuseLight = vec3(0.0, 0.0, 0.0);\n"
        for(var i in lights.nodes) {
            output += "$diffuseLight += applyLight($lights[" + i + "], $normal, $position, $color);\n"
        }
        return output
    }
    result: "$diffuseLight"
}
