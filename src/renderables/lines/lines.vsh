// BEGIN lines.vsh
layout(location=0) in vec3 position;

void main() {
    vec3 translatedPosition = position + cp_viewVector*0.1;
    gl_Position = cp_modelViewProjectionMatrix*vec4(translatedPosition, 1.0);
}
// END lines.vsh
