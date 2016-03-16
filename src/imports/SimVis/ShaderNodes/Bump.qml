import SimVis 1.0

ShaderNode {
    property bool invert: false
    property var strength: 1.0
    property var distance: 0.1
    property var height: Qt.vector2d(0.0, 0.0)
    property var normal: ShaderBuilderBinding {
        name: "normal"
        defaultValue: Qt.vector3d(0.0, 0.0, 1.0)
    }

    name: "bump"
    type: "vec3"
    result: "$(normal, vec3) + $(strength, float) * $(distance, float) * $(height, vec3)"
}
