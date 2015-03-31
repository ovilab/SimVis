attribute vec4 a_position;
attribute vec3 a_color;
attribute vec2 a_texcoord;
attribute float a_billboardId;
varying vec2 coords;
varying vec3 color;
varying vec3 vertexPosition;
varying float attenuationFactor;
varying float billboardId;

void main() {
    gl_Position = cp_modelViewProjectionMatrix*a_position;
    vertexPosition = a_position.xyz;
    billboardId = a_billboardId;
#ifdef DEFAULTLIGHT
    attenuationFactor = attenuation(vertexPosition);
#endif

    coords = a_texcoord;
    color = a_color;
}
