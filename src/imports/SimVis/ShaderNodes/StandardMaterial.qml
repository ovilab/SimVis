import SimVis 1.0

ShaderNode {
    property var color: "white"

    property var ambientColor: "black"
    property var diffuseColor: color
    property var specularColor: color
    property var ambientIntensity: 0.04
    property var diffuseIntensity: 1.0
    property var specularIntensity: 0.4
    property var hardness: 10.0

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
            output += "$this += standardMaterialLight($lights[" + i + "], $(normal, vec3), $(position, vec3), eyePosition,\n"
            output += "             $(ambientColor, vec3), $(diffuseColor, vec3), $(specularColor, vec3),\n"
            output += "             $(ambientIntensity, float), $(diffuseIntensity, float), $(specularIntensity, float),\n"
            output += "             $(hardness, float));\n"
        }
        return output
    }
    headerFile: "qrc:/SimVis/ShaderNodes/shaders/standard_material.glsl"
}
