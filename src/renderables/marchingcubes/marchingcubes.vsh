uniform highp mat4 modelViewProjectionMatrix;
uniform vec3 lightPosition;
uniform float lightFalloffDistance;
varying float light;
attribute highp vec4 a_position;
attribute highp vec3 a_normal;
varying highp vec3 normal;

void main() {
    gl_Position = modelViewProjectionMatrix*a_position;
    normal = a_normal;
    highp vec4 lightPositionProjected = modelViewProjectionMatrix*vec4(lightPosition, 1.0);
    highp float lightDistance = min(lightPositionProjected.z, gl_Position.z);
    light = 1.0;//clamp((lightFalloffDistance * 0.85 - lightDistance) / (lightFalloffDistance * 0.7), 0.4, 1.0);
}
