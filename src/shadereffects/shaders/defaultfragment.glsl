highp vec4 defaultFragment(highp vec3 normal, highp vec3 vertexPosition, highp vec3 color)
{
    highp vec3 light = vec3(1.0, 1.0, 1.0);
#ifdef SIMPLEXBUMP
    normal = simplexbump(normal, normal);
#endif

#ifdef DEFAULTLIGHT
    light = defaultLight(normal, vertexPosition, color);
#endif

#ifdef SKYBOXREFLECTION
    light += skyboxReflection(normal, vertexPosition);
#endif

    return vec4(color*light, 1.0);
}
