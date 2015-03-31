uniform float lightFalloffDistance;
uniform float normalVectorSign;
uniform float scale;

attribute vec4 a_position;
attribute vec3 a_normal;
attribute vec3 a_color;

varying float attenuationFactor;
varying vec3 normal;
varying vec3 color;
varying vec3 vertexPosition;

void main() {
    vertexPosition = a_position.xyz;
    vec4 scaledPosition = vec4(a_position.xyz*scale, a_position.w);

    normal = a_normal*normalVectorSign;
#ifdef DEFAULTLIGHT
    attenuationFactor = attenuation(vertexPosition);
#endif
    color = a_color;

    gl_Position = cp_modelViewProjectionMatrix*scaledPosition;
}
