uniform highp mat4 modelViewProjectionMatrix;
uniform vec3 lightPosition;
uniform float lightFalloffDistance;
uniform vec3 delta;
varying float light;
attribute highp vec4 a_position;
attribute highp vec3 a_normal;
attribute highp vec3 a_color;
varying highp vec3 normal;
varying highp vec3 color;

void main() {
    vec4 pos = a_position + vec4(delta, 0.0);
    gl_Position = modelViewProjectionMatrix*pos;
    normal = a_normal;
    color = a_color;
    highp vec4 lightPositionProjected = modelViewProjectionMatrix*vec4(lightPosition, 1.0);
    highp float lightDistance = min(lightPositionProjected.z, gl_Position.z);
    light = clamp((lightFalloffDistance * 0.85 - lightDistance) / (lightFalloffDistance * 0.7), 0.4, 1.0);
}
