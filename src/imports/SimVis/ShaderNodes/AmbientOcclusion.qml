import SimVis 1.0

ShaderNode {
    property var samples: 16
    property var radius: 0.5
    property var depthTexture
    property var position: ShaderBuilderBinding {
        property: "position"
        defaultValue: Qt.vector3d(0.0, 0.0, 0.0)
    }
    property var normal: ShaderBuilderBinding {
        property: "normal"
        defaultValue: Qt.vector3d(0.0, 0.0, 0.0)
    }

    name: "ambient_occlusion"
    type: "float"
    result: "
ambientOcclusion($(depthTexture, sampler2D), $(position, vec3),
    $(normal, vec3), $(samples, int), $(radius, float),
    viewMatrix, projectionMatrix)
"

    headerFile: "qrc:/SimVis/ShaderNodes/shaders/ambient-occlusion.glsl"
    // TODO make matrices explicit input
}
