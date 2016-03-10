// BEGIN trianglecollection.vsh
uniform highp float normalVectorSign;
layout(location=0) cp_in vec3 in_position;
layout(location=1) cp_in vec3 in_normal;
layout(location=2) cp_in vec3 in_color;

cp_out highp vec3 vs_normal;
cp_out highp vec3 vs_color;
cp_out highp vec3 vs_position;

void main() {
    vs_position = in_position.xyz;
    vs_normal = in_normal*normalVectorSign;
    vs_color = in_color;

    gl_Position = cp_modelViewProjectionMatrix*vec4(in_position, 1.0);
}
// END trianglecollection.vsh
