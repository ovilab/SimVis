import SimVis 1.0

ShaderNode {
    property var strength: 0.1
    property var color: Qt.vector3d(0.5, 0.5, 0.5)
    property var normal: ShaderBuilderBinding {
        property: "normal"
        defaultValue: Qt.vector3d(0.0, 0.0, 1.0)
    }

    name: "normalmap"
    type: "vec3"
    // $(strength, float)
    result: "normalMap($(normal, vec3), tangentMatrix[0], tangentMatrix[1], $(color, vec3), modelNormalMatrix)"
    header: "
vec3 normalMap(vec3 normal, vec3 tangent, vec3 binormal, vec3 color, mat3 modelNormalMatrix) {
    vec3 mcolor = 2.0 * color - vec3(1.0, 1.0, 1.0);
    vec3 result = normalize(mcolor[0] * tangent + mcolor[1] * binormal + mcolor[2] * normal);
    result = normalize(modelNormalMatrix * result);
    return result;
}
"
}
