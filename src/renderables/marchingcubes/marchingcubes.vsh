// BEGIN marchingcubes.vsh
uniform highp float normalVectorSign;
uniform highp float scale;

cp_in vec4 a_position;
cp_in vec3 a_normal;
cp_in vec3 a_color;

cp_out vec3 normal;
cp_out vec3 color;
cp_out vec3 position;

void main() {
    position = a_position.xyz;
    highp vec4 scaledPosition = vec4(a_position.xyz*scale, a_position.w);

    normal = a_normal*normalVectorSign;
    color = a_color;

    gl_Position = cp_modelViewProjectionMatrix*scaledPosition;
}
// END marchingcubes.vsh
