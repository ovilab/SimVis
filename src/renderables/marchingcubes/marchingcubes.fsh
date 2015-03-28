varying highp vec3 normal;
varying highp float light;
varying highp vec3 color;
varying highp vec3 vertexPosition;
varying highp float attenuationFactor;

void main() {
    lowp float n = 1.0;
    highp vec3 light = vec3(color);
    highp vec3 normal2 = normal;

#ifdef SIMPLEXBUMP
    normal2 = simplexbump(normal, vertexPosition);
#endif

#ifdef DEFAULTLIGHT
    light = vec3(ambient(color) + attenuationFactor*(diffuse(normal2, vertexPosition, color) + specular(normal2, vertexPosition, cp_specularColor)));
#endif

#ifdef SIMPLEXTEXTURE
    n = surface(vertexPosition); // From simplextexture.fsh
#endif

    gl_FragColor = vec4(light*n, 1.0);
}
