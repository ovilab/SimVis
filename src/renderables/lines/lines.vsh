// BEGIN lines.vsh
layout(location=0) in vec3 position;
uniform mat4 cp_mvpInv;
void main() {
    vec3 translatedPosition = position;
    // gl_Position = cp_modelViewProjectionMatrix*vec4(translatedPosition, 1.0);
    vec4 p_inv = cp_mvpInv*vec4(position, 1.0);
    gl_Position = cp_modelViewProjectionMatrix*p_inv;
}
// END lines.vsh
