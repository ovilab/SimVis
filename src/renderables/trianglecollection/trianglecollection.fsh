// BEGIN trianglecollection.fsh
in highp vec3 normal;
in highp vec3 color;
in highp vec3 vertexPosition;
uniform float alpha;
out vec4 outColor;
void main() {
    lowp float n = 1.0;
    highp vec3 light = vec3(color);
    highp vec3 normal2 = normal;

#ifdef SIMPLEXBUMP
    normal2 = simplexbump(normal, vertexPosition);
#endif

#ifdef DEFAULTLIGHT
    light = defaultLight(normal2, vertexPosition, color);
#endif

#ifdef SKYBOXREFLECTION
    light += skyboxReflection(normal2, vertexPosition);
#endif

#ifdef SIMPLEXTEXTURE
    n = surface(vertexPosition); // From simplextexture.fsh
#endif

    outColor = vec4(light*n, alpha);
}
// END trianglecollection.fsh
