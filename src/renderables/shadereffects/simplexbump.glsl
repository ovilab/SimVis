#ifndef SIMPLEXBUMP
#define SIMPLEXBUMP
uniform highp float cp_bumpIntensity;
uniform highp float cp_bumpScale;

highp vec3 simplexbump(highp vec3 normal, highp vec3 vertexPosition) {
    highp float nx = snoise(0.5*(vertexPosition.xy + vertexPosition.zz)*cp_bumpScale);
    highp float ny = snoise(0.5*(vertexPosition.yz + vertexPosition.xx)*cp_bumpScale);
    highp float nz = snoise(0.5*(vertexPosition.xz + vertexPosition.yy)*cp_bumpScale);
    return normal + vec3(nx, ny, nz)*cp_bumpIntensity;
}
#endif
