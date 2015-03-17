uniform highp mat4 modelViewProjectionMatrix;
attribute highp vec4 a_position;
attribute highp vec3 a_color;
attribute highp vec2 a_texcoord;
varying highp vec2 coords;
varying highp float light;
varying highp vec3 color;

void main() {
    gl_Position = modelViewProjectionMatrix*a_position;
    coords = a_texcoord;
    color = a_color;
    light = clamp(0.5*gl_Position.z - 0.5, 0.0, 1.0);
}
