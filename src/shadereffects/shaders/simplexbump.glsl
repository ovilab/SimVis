#ifndef SIMPLEXBUMPGLSL
#define SIMPLEXBUMPGLSL
uniform float cp_bumpIntensity;
uniform float cp_bumpScale;

vec3 simplexbump(vec3 normal, vec3 vertexPosition) {
    float nx = snoise(0.5*(vertexPosition.xy + vertexPosition.zz + vec2(2.0,2.0))*cp_bumpScale);
    float ny = snoise(0.5*(vertexPosition.yz + vertexPosition.xx + vec2(-2.0,2.0))*cp_bumpScale);
    float nz = snoise(0.5*(vertexPosition.xz + vertexPosition.yy + vec2(2.0,-2.0))*cp_bumpScale);
    return normal + vec3(nx, ny, nz)*cp_bumpIntensity;
}

#endif
