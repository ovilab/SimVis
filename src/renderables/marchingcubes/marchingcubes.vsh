uniform highp mat4 modelViewProjectionMatrix;
uniform highp vec3 lightPosition;
uniform highp float lightFalloffDistance;
uniform highp float normalVectorSign;

attribute highp vec4 a_position;
attribute highp vec3 a_normal;
attribute highp vec4 a_color;

varying highp float light;
varying highp vec3 normal;
varying highp vec4 color;

void main() {
    gl_Position = modelViewProjectionMatrix*a_position;
    normal = a_normal*normalVectorSign;
    color = a_color;
    highp vec4 lightPositionProjected = modelViewProjectionMatrix*vec4(lightPosition, 1.0);
    highp float lightDistance = lightPositionProjected.z;
    light = clamp((lightFalloffDistance * 0.85 - lightDistance) / (lightFalloffDistance * 0.7), 0.4, 1.0);
}
