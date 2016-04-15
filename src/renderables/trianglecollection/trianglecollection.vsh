// BEGIN trianglecollection.vsh
uniform highp float normalVectorSign;
layout(location=0) in vec4 a_position;
layout(location=1) in vec3 a_normal;
layout(location=2) in vec3 a_color;

uniform mat4 cp_mvpInv;

out highp vec3 normal;
out highp vec3 color;
out highp vec3 vertexPosition;

void main() {
    vec4 p = cp_projectionMatrixInverse*a_position;
    // vertexPosition = p.xyz;
    vertexPosition = a_position.xyz;
    normal = -a_normal*normalVectorSign;
    color = a_color;

    // gl_Position = cp_modelViewProjectionMatrix*a_position;
    vec4 p_inv = cp_mvpInv*p;
    gl_Position = cp_modelViewProjectionMatrix*p_inv;
    // gl_Position = p;
}
// END trianglecollection.vsh
