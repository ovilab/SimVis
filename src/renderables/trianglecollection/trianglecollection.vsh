// BEGIN trianglecollection.vsh
uniform highp float normalVectorSign;
layout(location=0) in vec4 a_position;
layout(location=1) in vec3 a_normal;
layout(location=2) in vec3 a_color;

out highp vec3 normal;
out highp vec3 color;
out highp vec3 vertexPosition;

void main() {
    vertexPosition = a_position.xyz;
    normal = a_normal*normalVectorSign;
    color = a_color;

    gl_Position = cp_modelViewProjectionMatrix*a_position;
}
// END trianglecollection.vsh
