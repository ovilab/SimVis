#ifndef SIMPLEXBUMP
#define SIMPLEXBUMP
uniform highp float cp_bumpIntensity;
uniform highp float cp_bumpScale;

highp vec3 simplexbump(highp vec3 normal, highp vec3 vertexPosition) {
    highp float nx = snoise(vertexPosition.xy*cp_bumpScale);
    highp float ny = snoise(vertexPosition.xz*cp_bumpScale);
    highp float nz = snoise(vertexPosition.yz*cp_bumpScale);
    return normal + vec3(nx, ny, nz)*cp_bumpIntensity;
}
#endif
