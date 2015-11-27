// BEGIN trianglecollection.vsh
uniform highp float normalVectorSign;

attribute highp vec4 a_position;
attribute highp vec3 a_normal;
attribute highp vec3 a_color;

varying highp vec3 normal;
varying highp vec3 color;
varying highp vec3 vertexPosition;

void main() {
    vertexPosition = a_position.xyz;
    normal = a_normal*normalVectorSign;
    color = a_color;

    gl_Position = cp_modelViewProjectionMatrix*a_position;
}
// END trianglecollection.vsh
