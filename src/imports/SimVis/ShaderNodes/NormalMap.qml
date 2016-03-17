import SimVis 1.0

ShaderNode {
    property var strength: 0.1
    property var color: Qt.vector3d(0.0, 0.0, 0.0)
    property var normal: ShaderBuilderBinding {
        property: "normal"
        defaultValue: Qt.vector3d(0.0, 0.0, 1.0)
    }

    name: "normalmap"
    type: "vec3"
    result: "$(normal, vec3) + $(strength, float) * (-1.0 + 2.0 * $(color, vec3))"
}
