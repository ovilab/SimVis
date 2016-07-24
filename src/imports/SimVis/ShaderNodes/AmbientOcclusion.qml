import SimVis 1.0

ShaderNode {
    property var samples: 16
    property var radius: 0.5
    property var noiseScale: 1.0
    property var depthTexture
    property var noiseTexture
    property var position: ShaderBuilderBinding {
        property: "position"
        defaultValue: Qt.vector3d(0.0, 0.0, 0.0)
    }
    property var normal: ShaderBuilderBinding {
        property: "normal"
        defaultValue: Qt.vector3d(0.0, 0.0, 0.0)
    }

    property string mode: "hemisphere"

    property string sphereResult: "
ambientOcclusion(
    $(depthTexture, sampler2D), $(noiseTexture, sampler2D),
    $(position, vec3), $(normal, vec3),
    $(samples, int), $(radius, float), 10.0 * $(noiseScale, float),
    viewMatrix, projectionMatrix)
"

    property string hemisphereResult: "
hemisphereAmbientOcclusion(
    $(depthTexture, sampler2D), $(noiseTexture, sampler2D),
    $(position, vec3), $(normal, vec3),
    $(samples, int), $(radius, float), 10.0 * $(noiseScale, float),
    viewMatrix, projectionMatrix)
"

    name: "ambient_occlusion"
    type: "float"
    result: mode === "hemisphere" ? hemisphereResult : sphereResult

    headerFile: "qrc:/SimVis/ShaderNodes/shaders/ambient-occlusion.glsl"
    // TODO make matrices explicit input
}
