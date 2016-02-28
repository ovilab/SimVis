import SimVis 1.0

ShaderNode {
    property var color: "white"
    property var normal: Qt.vector3d(0.0, 0.0, 0.0)
    property var position: Qt.vector3d(0.0, 0.0, 0.0)
    property ShaderGroup lights: ShaderGroup {
        Light {}
    }

    name: "diffuse"
    type: "vec3"
    source: {
        var output = ""
        output += "$this = vec3(0.0, 0.0, 0.0);\n"
        for(var i in lights.nodes) {
            output += "$this += applyLight($lights[" + i + "], $(normal, vec3), $(position, vec3), $(color, vec3));\n"
        }
        return output
    }
}
