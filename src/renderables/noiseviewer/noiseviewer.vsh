// BEGIN noiseviewer.vsh
layout(location=0) in vec3 position;
out vec3 vs_vertexPosition;

void main() {
    gl_Position = cp_modelViewProjectionMatrix * vec4(position, 1.0);
    vs_vertexPosition = position;
}
// END noiseviewer.vsh
