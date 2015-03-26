uniform highp mat4 modelViewProjectionMatrix;
uniform highp vec3 lightPosition;
uniform highp float lightFalloffDistance;
uniform highp float normalVectorSign;
uniform highp float scale;

attribute highp vec4 a_position;
attribute highp vec3 a_normal;
attribute highp vec3 a_color;

varying highp float light;
varying highp vec3 normal;
varying highp vec3 color;
varying highp vec4 scaledPosition;

vec3 deltaNormal(vec3 pos) {
    const float epsilon = 0.01;
    const float oneOverEpsilon = 100.0;
    float f0 = snoise(pos);
    float fx = snoise(vec3(pos.x + epsilon, pos.yz));
    float fy = snoise(vec3(pos.x, pos.y + epsilon, pos.z));
    float fz = snoise(vec3(pos.xy, pos.z + epsilon));
    return vec3( (fx - f0), (fy - f0), (fz - f0))*oneOverEpsilon;
}

void main() {
    scaledPosition = vec4(a_position.xyz*scale, a_position.w);
    gl_Position = modelViewProjectionMatrix*scaledPosition;
    normal = a_normal*normalVectorSign;// - deltaNormal(a_position.xyz*scale);

    color = a_color;
    highp vec4 lightPositionProjected = modelViewProjectionMatrix*vec4(lightPosition*scale, 0.0);
    highp float lightDistance = lightPositionProjected.z;
    light = clamp((lightFalloffDistance * 0.85 - lightDistance) / (lightFalloffDistance * 0.7), 0.4, 1.0);
}
